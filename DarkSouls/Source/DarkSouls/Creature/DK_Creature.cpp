// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Tool/Define.h"
#include "Tool/Struct.h"
#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Component/Stat/DK_StatComponent.h"
#include "UI/DK_WidgetComponent.h"
#include "UI/DK_HPBarWidget.h"



// Sets default values
ADK_Creature::ADK_Creature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RootCreatureCol"));

	// Mesh
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;

	GetCharacterMovement()->GroundFriction = 8.f;
	GetCharacterMovement()->FallingLateralFriction = 5.f;

	// ComboComponent
	ComboComponent = CreateDefaultSubobject<UDK_ComboComponent>(TEXT("ComboComponent"));


	// CollisionManagerComponent
	CollisionManagerComponent = CreateDefaultSubobject<UDK_CollisionManagerComponent>(TEXT("CollisionManagerComponent"));


	//// StatComponent
	//StatComponent = CreateDefaultSubobject<UDK_StatComponent>(TEXT("StatComponent"));


	// Widget
	WidgetComponent = CreateDefaultSubobject<UDK_WidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(RootComponent);
	WidgetComponent->SetCollisionProfileName(TEXT("NoCollision"));
}

void ADK_Creature::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

// Called when the game starts or when spawned
void ADK_Creature::BeginPlay()
{
	Super::BeginPlay();


	StatComponent->AddChangeHPDelegateFunc(WidgetComponent->GetWidget(), FName("UpdateHpBar"));
	StatComponent->ResetStat();
}

// Called every frame
void ADK_Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타이머로 할려고했는데, 카메라가 흔들리는 문제가 생김
	if (bIsSmoothTurn)
	{
		SmoothTurnTick();
	}


}





void ADK_Creature::AddImpulse(FVector Dir, float Powar)
{
	Dir.Normalize();
	GetCharacterMovement()->AddImpulse(Dir * Powar, true);
	LaunchCharacter(Dir * Powar, true, true);
}






void ADK_Creature::OnOffSceenHPBar(bool bIsOn)
{
	WidgetComponent->SetHiddenInGame(!bIsOn);
}

void ADK_Creature::Attack()
{
	if (!CanAttack())
		return;

	ResetInfoOnAttack();

	ComboComponent->ProcessComboCommand();
}

void ADK_Creature::BeginAttackRange_Notify()
{
	bIsInAttackRange = true;
}

void ADK_Creature::EndAttackRange_Notify()
{
	bIsInAttackRange = false;
	CollisionManagerComponent->ClearActorTemps();
}

void ADK_Creature::BeginColRange_Notify()
{
	const TArray<FString>& AttackCollisionInfos = ComboComponent->GetCurrentAttackCollisionInfos();
	CollisionManagerComponent->TurnAttackCol(AttackCollisionInfos, true);

}

void ADK_Creature::EndColRange_Notify()
{
	CollisionManagerComponent->TurnBlockAllCol();

	// Dodge 무효 델리게이트 호출
}

FAttackDamagedInfo ADK_Creature::GetCurrentAttackInfos()
{
	return ComboComponent->GetCurrentAttackInfos();
}



//float ADK_Creature::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	OnDamaged();
//
//	return 0.0f;
//}

void ADK_Creature::OnDamaged(const FAttackDamagedInfo& AttackDamagedInfo, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("%s is Attacked"), *GetName()));

	if (!CanDamaged())
		return;


	FVector CauserPos = DamageCauser->GetActorLocation();
	// 가드 여부
	if (CheckBlock(CauserPos))
	{
		BlockAttack(DamageCauser, AttackDamagedInfo.BlockPushPowar);

		return;
	}


	// 스턴 여부
	FVector TargetToMeDir = GetActorLocation() - CauserPos;
	TargetToMeDir.Normalize();

	if (!AttackDamagedInfo.bIsDown)
	{
		Stun(AttackDamagedInfo.StunTime, AttackDamagedInfo.bSetStunTimeToHitAnim);
		AddImpulse(TargetToMeDir, AttackDamagedInfo.HitPushPowar);
	}
	else
	{
		KnockDown(AttackDamagedInfo.StunTime);
		AddImpulse(TargetToMeDir, AttackDamagedInfo.KnockDownPushPowar);

		SmoothTurnByCallOnce(CauserPos, 10.f);
	}


	// 스탯
	StatComponent->DecreaseHP(AttackDamagedInfo.Damage);

}








void ADK_Creature::Stun(float StunTime, bool bSetAnimTime)
{
	if (!CanStun())
		return;

	if (bSetAnimTime)
	{
		const int32 HitAnimMontageIndex = 0;
		float StartTime = 0.f, EndTime = 0.f;
		HitMontage->GetSectionStartAndEndTime(HitAnimMontageIndex, StartTime, EndTime);
		StunTime = EndTime - StartTime - 0.1f;
	}
	

	ResetInfoOnStun();

	bIsStun = true;

	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ADK_Creature::EndStun, StunTime, false);
	
	if (HitMontage)
	{
		PlayAnimMontage(HitMontage, 1.f);
	}
	
}

void ADK_Creature::EndStun()
{
	bIsStun = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(HitMontage))
		StopAnimMontage(HitMontage);
}



void ADK_Creature::KnockDown(float KnockDownTime)
{
	if (!CanKnockDown())
		return;


	ResetInfoOnKnockDown();

	bIsKnockDown = true;

	// TODO : 일어서는 모션 시간을 하드코딩으로 했음, 시간되면 
	GetWorldTimerManager().ClearTimer(KnockDownTimerHandle);
	GetWorldTimerManager().SetTimer(KnockDownTimerHandle, this, &ADK_Creature::EndKnockDown, KnockDownTime, false);

	if (StartKnockDownMontage)
	{
		PlayAnimMontage(StartKnockDownMontage, 1.f);
	}

}

void ADK_Creature::EndKnockDown()
{
	// 일어서는 모션에서 EndKnockDown_Notify 호출
	bIsPlayEndKnockDown = true;
}

void ADK_Creature::EndKnockDown_Notify()
{
	bIsKnockDown = false;
	bIsPlayEndKnockDown = false;
}






void ADK_Creature::SmoothTurnByCallOnce(FVector InDestPos, float TurnSpeed)
{
	if (!CanSmoothTurn())
		return;

	DestPos = InDestPos;
	SmoothTurnSpeed = TurnSpeed;

	bIsSmoothTurn = true;
}

void ADK_Creature::StopSmoothTurn()
{
	bIsSmoothTurn = false;
}

void ADK_Creature::SmoothTurnTick()
{
	if (!CanSmoothTurn())
	{
		StopSmoothTurn();
		return;
	}


	FRotator CurRot = K2_GetActorRotation();
	CurRot.Roll = 0.f;
	CurRot.Pitch = 0.f;
	FRotator DestRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DestPos);
	DestRot.Roll = 0.f;
	DestRot.Pitch = 0.f;

	FRotator NowRot = FMath::RInterpTo(CurRot, DestRot, GetWorld()->DeltaTimeSeconds, SmoothTurnSpeed);

	FRotator DisRot = CurRot - NowRot;
	if (DisRot.IsNearlyZero(0.1f))
	{
		StopSmoothTurn();
		return;
	}

	SetActorRotation(NowRot);
	
}






void ADK_Creature::Dodge()
{
	if (!CanDodge())
		return;

	ResetInfoOnDodge();

	bIsDodge = true;

	PlayAnimMontage(DodgeMontage);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADK_Creature::EndDoge);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
}

void ADK_Creature::EndDoge(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsDodge = false;


}

void ADK_Creature::BeginDodgeSkip_Notify()
{
	bCanDodgeSkip = true;
}

void ADK_Creature::EndDodgeSkip_Notify()
{
	bCanDodgeSkip = false;
}






void ADK_Creature::BlockAttack(AActor* Attacker, float PushBackPowar)
{
	HitBlock();
	AddImpulse(GetActorForwardVector() * -1.f, PushBackPowar);

}

void ADK_Creature::Block()
{
	if (!CanBlock())
		return;

	ResetInfoOnBlock();

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;

	bIsBlock = true;
	
}

void ADK_Creature::EndBlock()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	StopSmoothTurn();

	bIsBlock = false;
}

bool ADK_Creature::CheckBlock(FVector AttackerPos)
{
	if (!bIsBlock)
		return false;

	const float MaxAngle = 90.f;


	FVector MeLook = GetActorForwardVector();
	FVector LookAtTargetVec = AttackerPos - GetActorLocation();
	LookAtTargetVec.Normalize();
	MeLook.Normalize();
	
	float  DotResult = FVector::DotProduct(MeLook, LookAtTargetVec);
	float Angle = (FMath::Acos(DotResult) / UE_PI) * 180.f;

	if (FMath::Abs(Angle) > MaxAngle)
		return false;


	return true;
}

void ADK_Creature::HitBlock()
{
	if (!bIsBlock)
		return;

	ResetInfoOnHitBlock();

	bIsHitBlock = true;
	
	float HitDelayTime = HitTime;

	if (BlockHitMontage)
	{
		float StartTime = 0.f, EndTime = 0.f;
		BlockHitMontage->GetSectionStartAndEndTime(0, StartTime, EndTime);
		HitDelayTime = EndTime - StartTime;

		PlayAnimMontage(BlockHitMontage, 1.f);
	}

	GetWorldTimerManager().ClearTimer(HitBlockTimerHandle);
	GetWorldTimerManager().SetTimer(HitBlockTimerHandle, this, &ADK_Creature::EndHitBlock, HitDelayTime, false);


}

void ADK_Creature::EndHitBlock()
{
	bIsHitBlock = false;

}






bool ADK_Creature::CanAttack()
{
	if (bIsStun)
		return false;

	if (bIsKnockDown)
		return false;

	if (bIsDodge)
		return false;

	if (bIsHitBlock)
		return false;


	return true;
}

bool ADK_Creature::CanDodge()
{
	if (bIsDodge)
		return false;

	if (bIsStun)
		return false;

	if (bIsHitBlock)
		return false;

	if (bIsKnockDown)
		return false;


	return true;
}

bool ADK_Creature::CanSmoothTurn()
{
	if (bIsStun)
		return false;

	if (bIsHitBlock)
		return false;

	//if (bIsKnockDown)
	//	return false;

	if (bIsDodge)
		return false;


	return true;
}

bool ADK_Creature::CanStun()
{
	if (bIsKnockDown)
		return false;


	return true;
}

bool ADK_Creature::CanKnockDown()
{
	if (bIsKnockDown)
		return false;

	return true;
}

bool ADK_Creature::CanDamaged()
{
	if (bCanDodgeSkip)
		return false;

	if (bIsKnockDown)
		return false;

	return true;
}

bool ADK_Creature::CanBlock()
{
	if (bIsDodge)
		return false;

	if (bIsStun)
		return false;

	if (bIsKnockDown)
		return false;


	return true;
}





void ADK_Creature::ResetInfoOnAttack()
{
	EndBlock();

}

void ADK_Creature::ResetInfoOnStun()
{
	EndColRange_Notify(); // 공격 콜라이더
	EndAttackRange_Notify(); // 공격 애니메이션 범위
	ComboComponent->ResetComboInfo(); // 콤보

	EndDodgeSkip_Notify(); // 무적시간 (혹시모르니)

	StopSmoothTurn();

	EndBlock();
}

void ADK_Creature::ResetInfoOnKnockDown()
{
	ResetInfoOnStun();

	EndStun();

	EndBlock();
}

void ADK_Creature::ResetInfoOnDodge()
{
	EndBlock();

}

void ADK_Creature::ResetInfoOnBlock()
{
}

void ADK_Creature::ResetInfoOnHitBlock()
{
	StopSmoothTurn();
}
