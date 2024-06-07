// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Player.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
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
#include "Player/DK_PlayerController.h"
#include "UI/DK_HUDWidget.h"
#include "Component/Stat/DK_PlayerStatComponent.h"



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


	// StatComponent
	PlayerStatComponent = CreateDefaultSubobject<UDK_PlayerStatComponent>(TEXT("PlayerStatComponent"));
	StatComponent = PlayerStatComponent;


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


	ADK_PlayerController* PlayerController = Cast<ADK_PlayerController>(GetController());

	//APlayerController* PlayerController = Cast<APlayerController>(GetController());
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


	UDK_HPBarWidget* HudHpBarWidget = PlayerController->GetHUDWidget()->GetHpBarWidget();
	PlayerStatComponent->AddChangeHPDelegateFunc(HudHpBarWidget, FName("UpdateHpBar"));
	
	UDK_HPBarWidget* HudSpBarWidget = PlayerController->GetHUDWidget()->GetSpBarWidget();
	PlayerStatComponent->AddChangeSPDelegateFunc(HudSpBarWidget, FName("UpdateHpBar"));
	PlayerStatComponent->ResetStat();


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

	if (bIsBlock)
	{
		// 입력 벡터를 엑터와 컨트롤러의 Yaw 차이 만큼 돌려주는 작업
		FRotator ActorRot = GetActorRotation();
		FVector MoveInputDir3D = FVector(MoveInputDir.X, MoveInputDir.Y, 0.f);
		MoveInputDir3D.Normalize();
		FVector Rot = UKismetMathLibrary::RotateAngleAxis(MoveInputDir3D, Rotation.Yaw - ActorRot.Yaw, FVector::ZAxisVector);
		BlockMoveDir.X = Rot.X;
		BlockMoveDir.Y = Rot.Y;
	}

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


	// 락온일 때는 적 방향으로 공격
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





void ADK_Player::Stun(float StunTime, bool bSetAnimTime)
{
	Super::Stun(StunTime, bSetAnimTime);


}

void ADK_Player::EndStun()
{
	Super::EndStun();

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

	// LockOnPos 위치를 가리키는 Arrow 컴포넌트를 찾는다
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


	// 거리 확인
	if (LockMaxDis < GetDistanceTo(TargetLockedOn.Get()))
	{
		LockTarget(false);
		return;
	}


	// 컨트롤러(Boom)의 Yaw을 얼마나 회전해야하냐
	FVector ArrowLocation = LockArrow->GetComponentLocation();
	FVector PlayerLocation = GetActorLocation();
	
	FRotator RotationYaw = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, ArrowLocation);
	
	// 컨트롤러(Boom)의 Pitch을 얼마나 회전해야하냐
	FVector StartLocation;
	FVector LocalArmLocation = FVector(CameraBoom->TargetArmLength * -1.f, 0.f, 0.f);
	LocalArmLocation = UKismetMathLibrary::RotateAngleAxis(LocalArmLocation, RotationYaw.Yaw, FVector::ZAxisVector);
	StartLocation.X = LocalArmLocation.X;
	StartLocation.Y = LocalArmLocation.Y;
	StartLocation.Z = CameraBoom->TargetOffset.Z;

	FVector EndLocation = ArrowLocation - PlayerLocation;
	FRotator RotationPitch = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);


	// 구한 Yaw와 Pitch로 Lerp해서 회전한다
	float Pitch = RotationPitch.Pitch;
	float Yaw = RotationYaw.Yaw;

	FRotator CurRot = GetController()->GetControlRotation();
	FRotator WantRot(Pitch , Yaw, 0.f);


	FRotator ResultRot = FMath::RInterpTo(CurRot, WantRot, GetWorld()->GetDeltaSeconds(), LockOnSmoothSpeed);


	GetController()->SetControlRotation(ResultRot);

}






void ADK_Player::Block()
{
	if (!CanBlock())
		return;

	ResetInfoOnBlock();

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;

	// 락온일 때는 적 방향으로 공격
	if (bIsTargetLockOn && IsValid(TargetLockedOn.Get()))
	{
		SmoothTurnByCallOnce(TargetLockedOn.Get()->GetActorLocation(), 30.f);
	}

	// 처음 블락 진입
	if (!bIsBlock)
	{
		StartPerfectBlock();
	}

	bIsBlock = true;
}


void ADK_Player::BlockAttack(AActor* Attacker, float PushBackPowar)
{
	if (bIsPerfectBlock)
	{
		ADK_Creature* AttackerCreature = Cast<ADK_Creature>(Attacker);
		if (AttackerCreature)
		{
			// TODO : 적 그로기 수치 쌓기, 스테미나 덜 깎기
			AttackerCreature->Stun(0.f, true);
		}

	}


	// TODO : 스테미나에 따라 위크 호출
	HitBlock();
	
	/*HitWeakBlock();*/


	AddImpulse(GetActorForwardVector() * -1, PushBackPowar);

}

void ADK_Player::StartPerfectBlock()
{
	const float PerfectBlockTime = 0.05f;

	EndPerfectBlock();

	bIsPerfectBlock = true;

	GetWorldTimerManager().SetTimer(PerfectBlockTimerHandle, this, &ADK_Player::EndPerfectBlock, PerfectBlockTime, false);

}

void ADK_Player::EndPerfectBlock()
{
	bIsPerfectBlock = false;
	GetWorldTimerManager().ClearTimer(PerfectBlockTimerHandle);

}

void ADK_Player::HitWeakBlock()
{
	if (!bIsBlock)
		return;

	ResetInfoOnHitBlock();

	bIsHitBlock = true;

	float HitDelayTime = HitTime;

	if (BlockWeakHitMontage)
	{
		float StartTime = 0.f, EndTime = 0.f;
		BlockWeakHitMontage->GetSectionStartAndEndTime(0, StartTime, EndTime);
		HitDelayTime = EndTime - StartTime;

		PlayAnimMontage(BlockWeakHitMontage, 1.f);
	}

	GetWorldTimerManager().ClearTimer(HitBlockTimerHandle);
	GetWorldTimerManager().SetTimer(HitBlockTimerHandle, this, &ADK_Creature::EndHitBlock, HitDelayTime, false);

	// 가드 풀기
	EndBlock();

	// 스테미나 딜레이 함수 호출 (몇초동안 회복하는 걸 멈춘다)
	// 그럼 Block에서 스테미나가 없기 때문에 가드를 다시 못 쓴다
}








bool ADK_Player::CanAttack()
{
	if (!Super::CanAttack())
		return false;

	if (bIsAttacking)
		return false;


	// 기를 안 모으고 있는데, 키가 눌린 상태
	// *누른상태에서 파워어택이 나갈경우 다시 입력 들어오는거 막기 위해서
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

	if (bIsHitBlock)
		return false;

	if (bIsKnockDown)
		return false;


	// * 루트 모션 중이라 안 움직이는 뜻
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

bool ADK_Player::CanSmoothTurn()
{
	if (!Super::CanSmoothTurn())
		return false;

	return true;
}

bool ADK_Player::CanStun()
{
	if (!Super::CanStun())
		return false;

	return true;
}

bool ADK_Player::CanKnockDown()
{
	if (!Super::CanKnockDown())
		return false;

	return true;
}

bool ADK_Player::CanDamaged()
{
	if (!Super::CanDamaged())
		return false;

	return true;
}




void ADK_Player::ResetInfoOnAttack()
{
	Super::ResetInfoOnAttack();

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnStun()
{
	Super::ResetInfoOnStun();

	// 차지중에 스턴 걸리면 차지 리셋
	ResetChargeAttack();
	// 공격도중에 스턴 걸리면 노티 끊기니까
	bIsAttacking = false;

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnKnockDown()
{
	Super::ResetInfoOnKnockDown();

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnDodge()
{
	Super::ResetInfoOnDodge();

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnBlock()
{
	Super::ResetInfoOnBlock();

	
}

void ADK_Player::ResetInfoOnHitBlock()
{
	Super::ResetInfoOnHitBlock();

}

