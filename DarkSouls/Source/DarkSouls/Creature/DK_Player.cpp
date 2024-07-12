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
#include "Manager/DK_UIManager.h"
#include "UI/DK_SmoothBarWidget.h"
#include "Component/Stat/DK_PlayerStatComponent.h"
#include "Component/Combo/DK_ComboComponent.h"



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


	APlayerController* PlayerController = Cast<APlayerController>(GetController());

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


	
	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GetUIManager()->BindEventForHUD(PlayerStatComponent);

	PlayerStatComponent->OnReleaseTPDelegate.AddUObject(this, &ADK_Player::GenRecoveryHP_Notify);
	PlayerStatComponent->ResetStat(NormalRecoverySPPerSec, DecreaseTPPerSec);


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

	if (StatComponent->GetIsBlocking())
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

	PlayerStatComponent->DelayRecoverySP(1.5f);

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
	ComboComponent->ChangeComboActionData(0);

	bIsAttacking = true;

	// CHECK: 가비지가 지우도록 넘기는데, 곧바로 직접 지워주는게 나은지 확인 필요. 
	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), SmallAttackAnim, 1.f, 0.f);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ADK_Player::EndAttack);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ADK_Player::EndAttack);
	PlayMontageCallbackProxy->OnNotifyBegin.AddDynamic(this, &ADK_Player::BeginNotifyAttack);
	PlayMontageCallbackProxy->OnNotifyEnd.AddDynamic(this, &ADK_Player::EndNotifyAttack);


	ResetChargeAttack();

	PlayerStatComponent->DelayRecoverySP(DelaySPTimeAfterActting);
	PlayerStatComponent->DecreaseSP(SmallAttackSP);

}

void ADK_Player::PowarAttack()
{
	ComboComponent->ChangeComboActionData(1);

	bIsAttacking = true;

	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), PowarAttackAnim, 1.f, 0.f);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ADK_Player::EndAttack);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ADK_Player::EndAttack);
	PlayMontageCallbackProxy->OnNotifyBegin.AddDynamic(this, &ADK_Player::BeginNotifyAttack);
	PlayMontageCallbackProxy->OnNotifyEnd.AddDynamic(this, &ADK_Player::EndNotifyAttack);

	
	ResetChargeAttack();

	PlayerStatComponent->DelayRecoverySP(DelaySPTimeAfterActting);
	PlayerStatComponent->DecreaseSP(PowarAttackSP);

}

void ADK_Player::ResetChargeAttack()
{
	ChargePowarTimeAcc = 0.f;
	bIsCharging = false;
}

void ADK_Player::EndAttack(FName NotifyName)
{
	ResetChargeAttack();
	bIsInAttackRange = false;
	bIsAttacking = false;
}

void ADK_Player::BeginNotifyAttack(FName NotifyName)
{
	if(NotifyName == TEXT("Check"))
	{
		bIsAttacking = false;
	}
	else if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}

}

void ADK_Player::EndNotifyAttack(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		EndAttackRange_Notify();
	}
}



void ADK_Player::Stun(float StunTime, bool bSetAnimTime)
{
	Super::Stun(StunTime, bSetAnimTime);


}

void ADK_Player::EndStun()
{
	Super::EndStun();

}





void ADK_Player::Dodge()
{
	if (!CanDodge())
		return;

	Super::Dodge();

	// 키 입력 방향으로 바로 회피
	FVector Dir(MoveInputDir.X, MoveInputDir.Y, 0.f);
	FRotator DirRot = UKismetMathLibrary::FindLookAtRotation(FVector::Zero(), Dir);
	DirRot.Pitch = 0.f;
	DirRot.Roll = 0.f;

	FRotator CamRot = GetController()->GetControlRotation();
	DirRot.Yaw = CamRot.Yaw + DirRot.Yaw;

	SetActorRotation(DirRot);


	// SP 소모
	PlayerStatComponent->DelayRecoverySP(DelaySPTimeAfterActting);
	PlayerStatComponent->DecreaseSP(DodgeSP);
}

void ADK_Player::OnDodgeSkip_Notify()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString(TEXT("sdfsdfsdfd")));
	PlayerStatComponent->IncreaseTP(IncreaseTPPer);
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


	// 락온일 때는 적 방향으로 공격
	if (bIsTargetLockOn && IsValid(TargetLockedOn.Get()))
	{
		SmoothTurnByCallOnce(TargetLockedOn.Get()->GetActorLocation(), 30.f);
	}

	// 처음 블락 진입
	if (!StatComponent->GetIsBlocking())
	{
		StartPerfectBlock();
	}

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;

	PlayerStatComponent->SetRecoverySPPerSecSpeed(SlowRecoverySPPerSec);

	StatComponent->SetIsBlocking(true);
}

void ADK_Player::EndBlock()
{
	Super::EndBlock();

	PlayerStatComponent->SetRecoverySPPerSecSpeed(NormalRecoverySPPerSec);

}


void ADK_Player::BlockAttack(bool bCanParrying, float KnockBackPowar, AActor* DamageCauser)
{
	if (bCanParrying && bIsPerfectBlock)
	{
		ADK_Creature* AttackerCreature = Cast<ADK_Creature>(DamageCauser);
		if (AttackerCreature)
		{
			// TODO
			const int32 BlockGPValue = 200;
			AttackerCreature->BeBlockedPerfectly(BlockGPValue);
		}

	}


	PlayerStatComponent->DelayRecoverySP(DelaySPTimeAfterActting);
	PlayerStatComponent->DecreaseSP(BlockSP);

	if (PlayerStatComponent->IsZeroSP())
	{
		HitWeakBlock();
	}
	else
	{
		HitBlock();
	}
	

	AddImpulse(GetActorForwardVector() * -1.f, KnockBackPowar);

}

void ADK_Player::StartPerfectBlock()
{
	const float PerfectBlockTime = 0.1f;

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
	if (!StatComponent->GetIsBlocking())
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






void ADK_Player::GenRecoveryHP_Notify()
{
	// TODO: 몬스터 위치 때리면 회복하는 스킬

	PRINT_TEXT(TEXT("GenRecoveryHP!!!"));
}






bool ADK_Player::CanAttack()
{
	if (!Super::CanAttack())
		return false;

	if (bIsAttacking)
		return false;

	if (!PlayerStatComponent->CanUse(AttackSPThreshould))
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

	if (!PlayerStatComponent->CanUse(DodgeSPThreshould))
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

	if (!PlayerStatComponent->CanUse(BlockSPThreshould))
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

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnKnockDown()
{
	// 스턴의 리셋까지 호출해 줌
	Super::ResetInfoOnKnockDown();

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnDodge()
{
	Super::ResetInfoOnDodge();

	// 차지중에 스턴 걸리면 차지 리셋
	ResetChargeAttack();

	EndPerfectBlock();
}

void ADK_Player::ResetInfoOnBlock()
{
	Super::ResetInfoOnBlock();

	// 블락 중에는 차지, 공격 가능

}

void ADK_Player::ResetInfoOnHitBlock()
{
	Super::ResetInfoOnHitBlock();


}

