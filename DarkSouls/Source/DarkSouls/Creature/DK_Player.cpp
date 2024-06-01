// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"

#include "Game/DK_GameMode.h"
#include "Manager/DK_OptionManager.h"

ADK_Player::ADK_Player()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	// Mesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Resources/Primitive_Characters_Pack/Mesh/Primitive_01/Mesh_UE5/Separate/SKM_Primitive_Charater_01_Body.SKM_Primitive_Charater_01_Body'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}



	// Input
	{
		static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContext(TEXT(
			"/Script/EnhancedInput.InputMappingContext'/Game/Inputs/IMC_Default.IMC_Default'"));
		if (InputMappingContext.Object)
		{
			DefaulMappingContext = InputMappingContext.Object;
		}

		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Jump.IA_Jump'"));
		if (nullptr != InputActionJumpRef.Object)
		{
			JumpAction = InputActionJumpRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttakRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Attack.IA_Attack'"));
		if (nullptr != InputActionAttakRef.Object)
		{
			AttackAction = InputActionAttakRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Move.IA_Move'"));
		if (nullptr != InputActionShoulderMoveRef.Object)
		{
			MoveAction = InputActionShoulderMoveRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Look.IA_Look'"));
		if (nullptr != InputActionShoulderLookRef.Object)
		{
			LookAction = InputActionShoulderLookRef.Object;
		}
	}


}

void ADK_Player::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

void ADK_Player::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->ClearAllMappings();

			if (DefaulMappingContext)
			{
				Subsystem->AddMappingContext(DefaulMappingContext, 0);
			}
		}
	}

}

void ADK_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &ADK_Player::ChargeAttack);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Completed, this, &ADK_Player::EndChargeAttack);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADK_Player::ShoulderMove);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADK_Player::ShoulderLook);

}

void ADK_Player::ShoulderMove(const FInputActionValue& Value)
{
	// 공격 중에는 움직임 X -> MovementComponent로 막으면 RootAnimation도 막힘
	if (!CanMove())
	{
		return;
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ADK_Player::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());
	const float MouseSensitivity = GameMode->GetOptionManager()->GetMouseSensitivity();

	AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
	AddControllerPitchInput(LookAxisVector.Y * MouseSensitivity);

	// UE_LOG(LogTemp, Log, TEXT("Log Message %f / %f"), LookAxisVector.X, LookAxisVector.Y);
}




void ADK_Player::ChargeAttack(const FInputActionValue& Value)
{
	if (!CanAttack())
	{
		return;
	}

	// 기를 안 모으고 있는데, 키가 눌린 상태
	// *누른상태에서 파워어택이 나갈경우 다시 입력 들어오는거 막기 위해서
	if (!bIsCharging && bIsHoldingAttackKey)
	{
		return;
	}

	bIsCharging = true;

	float DeltaSeconds = GetWorld()->GetDeltaSeconds();

	ChargePowarTimeAcc = FMath::Clamp(ChargePowarTimeAcc + DeltaSeconds, 0.f, MaxChargePowerTime);

	if (ChargePowarTimeAcc >= PowarAttackTimeThreshould)
	{
		PowarAttack();
	}

	bIsHoldingAttackKey = true;
}

void ADK_Player::EndChargeAttack(const FInputActionValue& Value)
{
	// 약 공격
	if (bIsCharging && ChargePowarTimeAcc < PowarAttackTimeThreshould)
	{
		SmallAttack();
	}

	bIsHoldingAttackKey = false;
}

void ADK_Player::SmallAttack()
{
	bIsAttacking = true;

	PlayAnimMontage(SmallAttackAnim);

	ResetChargeAttack();
}

void ADK_Player::PowarAttack()
{
	bIsAttacking = true;

	PlayAnimMontage(PowarAttackAnim);

	ResetChargeAttack();
}

void ADK_Player::ResetChargeAttack()
{
	ChargePowarTimeAcc = 0.f;
	bIsCharging = false;
}

void ADK_Player::Stun(float StunTime)
{
	Super::Stun(StunTime);


}

void ADK_Player::EndStun()
{
	Super::EndStun();

}

void ADK_Player::ResetInfoOnStun()
{
	Super::ResetInfoOnStun();

	// 차지중에 스턴 걸리면 차지 리셋
	ResetChargeAttack();
	// 공격도중에 스턴 걸리면 노티 끊기니까
	bIsAttacking = false;
}

void ADK_Player::CheckAttack_Notify()
{
	bIsAttacking = false;

}


bool ADK_Player::CanAttack()
{
	// 스턴이면
	if (bIsStun)
		return false;

	// 공격중일 때
	if (bIsAttacking)
		return false;

	return true;
}

bool ADK_Player::CanMove()
{
	if (bIsAttacking)
		return false;

	if (bIsCharging)
		return false;

	if (bIsStun)
		return false;

	return true;
}