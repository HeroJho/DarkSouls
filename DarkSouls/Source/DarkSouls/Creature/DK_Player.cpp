// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Tool/Define.h"
#include "Game/DK_GameMode.h"
#include "Manager/DK_OptionManager.h"
#include "Manager/DK_ToolManager.h"



ADK_Player::ADK_Player()
{
	PrimaryActorTick.bCanEverTick = true;

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
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTargetLockRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_TargetLock.IA_TargetLock'"));
		if (nullptr != InputActionTargetLockRef.Object)
		{
			TargetLockAction = InputActionTargetLockRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionDodgeRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Dodge.IA_Dodge'"));
		if (nullptr != InputActionDodgeRef.Object)
		{
			DodgeAction = InputActionDodgeRef.Object;
		}
		static ConstructorHelpers::FObjectFinder<UInputAction> InputActionBlockRef(TEXT(
			"/Script/EnhancedInput.InputAction'/Game/Inputs/Actions/IA_Block.IA_Block'"));
		if (nullptr != InputActionBlockRef.Object)
		{
			BlockAction = InputActionBlockRef.Object;
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

	EnhancedInputComponent->BindAction(TargetLockAction, ETriggerEvent::Started, this, &ADK_Player::LockTarget);
	EnhancedInputComponent->BindAction(DodgeAction, ETriggerEvent::Started, this, &ADK_Player::Dodge);

	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Triggered, this, &ADK_Creature::Block);
	EnhancedInputComponent->BindAction(BlockAction, ETriggerEvent::Completed, this, &ADK_Creature::EndBlock);
}

void ADK_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);



	if (bIsTargetLockOn)
	{
		LockTick();
	}


}

void ADK_Player::ShoulderMove(const FInputActionValue& Value)
{
	MoveInputDir = Value.Get<FVector2D>();

	if (!CanMove())
	{
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MoveInputDir.X);
	AddMovementInput(RightDirection, MoveInputDir.Y);
}

void ADK_Player::ShoulderLook(const FInputActionValue& Value)
{
	if (bIsTargetLockOn)
	{
		return;
	}

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

	ResetInfoOnAttack();

	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Blue, FString::Printf(TEXT("afaf")));

	// ������ ���� �� �������� ����
	if (bIsTargetLockOn && IsValid(TargetLockedOn.Get()))
	{
		SmoothTurnByCallOnce(TargetLockedOn.Get()->GetActorLocation(), 10.f);
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
	// �� ����
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

void ADK_Player::ResetInfoOnAttack()
{
	Super::ResetInfoOnAttack();

	EndBlock();

}

void ADK_Player::ResetInfoOnStun()
{
	Super::ResetInfoOnStun();

	// �����߿� ���� �ɸ��� ���� ����
	ResetChargeAttack();
	// ���ݵ��߿� ���� �ɸ��� ��Ƽ ����ϱ�
	bIsAttacking = false;
}

void ADK_Player::ResetInfoOnBlock()
{
	Super::ResetInfoOnBlock();

	// �����߿� ����
	// ResetChargeAttack();


}


void ADK_Player::CheckAttack_Notify()
{
	bIsAttacking = false;

}



void ADK_Player::Dodge()
{
	if (!CanDodge())
		return;

	Super::Dodge();

	FVector Dir(MoveInputDir.X, MoveInputDir.Y, 0.f);

	FRotator DirRot = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(), Dir);
	DirRot.Pitch = 0.f;
	DirRot.Roll = 0.f;


	FRotator CamRot = GetController()->GetControlRotation();
	DirRot.Yaw = CamRot.Yaw + DirRot.Yaw;

	SetActorRotation(DirRot);

}




void ADK_Player::LockTarget()
{
	if (bIsTargetLockOn)
	{
		LockTarget(false);
	}
	else
	{
		LockTarget(true);
	}

}

void ADK_Player::LockTarget(bool bIsLock)
{

	if (bIsLock)
	{
		ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());

		FVector StanPos = GetActorLocation();

		FRotator ControlRotation = GetController()->GetControlRotation();
		FVector StanVec = FRotationMatrix(ControlRotation).GetScaledAxis(EAxis::X);

		AActor* LockTarget = GameMode->GetToolManager()->GetObjectInNearstAngleByChannel(ECC_GameTraceChannel2, StanPos, LockMaxDis, StanVec, MaxAngle, false);
		if (LockTarget)
		{
			TargetLockedOn = LockTarget;

			CameraBoom->CameraLagSpeed = LockOnCameraLagSpeed;
			bIsTargetLockOn = true;

		}

	}
	else
	{
		TargetLockedOn = nullptr;

		CameraBoom->CameraLagSpeed = LockOffCameraLagSpeed;
		bIsTargetLockOn = false;

	}


}

void ADK_Player::LockTick()
{
	if(!IsValid(TargetLockedOn.Get()))
	{
		LockTarget(false);
		return;
	}

	// LockOnPos ��ġ�� ����Ű�� Arrow ������Ʈ�� ã�´�
	UArrowComponent* LockArrow = nullptr;
	TArray<UArrowComponent*> ArrowComponents;
	TargetLockedOn->GetComponents<UArrowComponent>(ArrowComponents);

	for (int32 i = 0; i < ArrowComponents.Num(); ++i)
	{
		if (ArrowComponents[i]->GetName() == TEXT("LockOnPos"))
		{
			LockArrow = ArrowComponents[i];
			break;
		}

	}

	if (LockArrow == nullptr)
	{
		LockTarget(false);
		return;
	}


	// �Ÿ� Ȯ��
	if (LockMaxDis < GetDistanceTo(TargetLockedOn.Get()))
	{
		LockTarget(false);
		return;
	}


	// ��Ʈ�ѷ�(Boom)�� Yaw�� �󸶳� ȸ���ؾ��ϳ�
	FVector ArrowLocation = LockArrow->GetComponentLocation();
	FVector PlayerLocation = GetActorLocation();
	
	FRotator RotationYaw = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, ArrowLocation);
	
	// ��Ʈ�ѷ�(Boom)�� Pitch�� �󸶳� ȸ���ؾ��ϳ�
	FVector StartLocation;
	FVector LocalArmLocation = FVector(CameraBoom->TargetArmLength * -1.f, 0.f, 0.f);
	LocalArmLocation = UKismetMathLibrary::RotateAngleAxis(LocalArmLocation, RotationYaw.Yaw, FVector::ZAxisVector);
	StartLocation.X = LocalArmLocation.X;
	StartLocation.Y = LocalArmLocation.Y;
	StartLocation.Z = CameraBoom->TargetOffset.Z;

	FVector EndLocation = ArrowLocation - PlayerLocation;
	FRotator RotationPitch = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);


	// ���� Yaw�� Pitch�� Lerp�ؼ� ȸ���Ѵ�
	float Pitch = RotationPitch.Pitch;
	float Yaw = RotationYaw.Yaw;

	FRotator CurRot = GetController()->GetControlRotation();
	FRotator WantRot(Pitch , Yaw, 0.f);


	FRotator ResultRot = FMath::RInterpTo(CurRot, WantRot, GetWorld()->GetDeltaSeconds(), LockOnSmoothSpeed);


	GetController()->SetControlRotation(ResultRot);

}






void ADK_Player::Block()
{
	Super::Block();

	// ������ ���� �� �������� ����
	if (bIsTargetLockOn && IsValid(TargetLockedOn.Get()))
	{
		SmoothTurnByCallOnce(TargetLockedOn.Get()->GetActorLocation(), 10.f);
	}

}





bool ADK_Player::CanAttack()
{
	if (bIsStun)
		return false;

	if (bIsKnockDown)
		return false;

	if (bIsAttacking)
		return false;

	if (bIsDodge)
		return false;

	// �⸦ �� ������ �ִµ�, Ű�� ���� ����
	// *�������¿��� �Ŀ������� ������� �ٽ� �Է� �����°� ���� ���ؼ�
	if (!bIsCharging && bIsHoldingAttackKey)
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

	if (bIsKnockDown)
		return false;


	// * ��Ʈ ��� ���̶� �� �����̴� ��
	//if (bIsDodge)
	//	return false;

	return true;
}

bool ADK_Player::CanDodge()
{
	if (!Super::CanDodge())
		return false;

	if (bIsAttacking)
		return false;

	if (bIsCharging)
		return false;


	return true;
}

bool ADK_Player::CanBlock()
{
	if (!Super::CanBlock())
		return false;

	if (bIsAttacking)
		return false;

	if (bIsCharging)
		return false;


	return true;
}
