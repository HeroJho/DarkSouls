// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayMontageCallbackProxy.h"

#include "Tool/Define.h"
#include "Tool/Struct.h"
#include "Game/DK_GameMode.h"
#include "Manager/DK_UIManager.h"
#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Component/Stat/DK_StatComponent.h"

#include "UI/DK_WidgetComponent.h"
#include "UI/DK_HUDWidget.h"
#include "UI/DK_SmoothBarWidget.h"


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

	OnOffHUDHPBar(false);
	OnOffScreenHPBar(false);

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




void ADK_Creature::OnOffScreenHPBar(bool bIsOn)
{
	WidgetComponent->SetHiddenInGame(!bIsOn);
}

void ADK_Creature::OnOffHUDHPBar(bool bIsOn)
{
	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());
	UDK_HUDWidget* HUDWidget = GameMode->GetUIManager()->GetHUD();
	if (HUDWidget == nullptr)
		return;

	if (bIsOn)
	{
		if (BossSmoothBarWidget.Get() != nullptr)
			return;

		UDK_SmoothBarWidget* BossHpBarWidget = nullptr;
		// 꽉 찻다
		FString Str;
		Str = Str.Printf(TEXT("가나다라"));
		FText Name = FText::FromString(Str);
		if (!HUDWidget->MakeBossHpBar(&BossHpBarWidget, Name))
			return;


		BossSmoothBarWidget = BossHpBarWidget;
		BossHpBarDelegateHandle = StatComponent->AddChangeHPDelegateFunc(BossSmoothBarWidget.Get(), FName("UpdateBar"));

		StatComponent->BroadcastStat();

		
	}
	else
	{
		HUDWidget->RemoveBossHpBar(BossSmoothBarWidget.Get());
		StatComponent->RemoveChangeHPDelegateFunc(BossHpBarDelegateHandle);
		
		BossHpBarDelegateHandle.Reset();
		BossSmoothBarWidget = nullptr;
	}
	
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

}

FAttackDamagedInfo ADK_Creature::GetCurrentAttackInfos()
{
	return ComboComponent->GetCurrentAttackInfos();
}

void ADK_Creature::InterruptedAttack_Notify()
{
	EndAttackRange_Notify();
	EndColRange_Notify();
}


void ADK_Creature::OnDamaged(const FAttackDamagedInfo& AttackDamagedInfo, AActor* DamageCauser)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("%s is Attacked"), *GetName()));

	// 대미지 입을 수 있는 상태 여부
	if (!CanDamaged())
		return;

	// 회피 여부
	if (bCanDodgeSkip)
	{
		PerfectDodge();
		return;
	}

	// 가드 여부
	FVector CauserPos = DamageCauser->GetActorLocation();
	if (CheckBlock(CauserPos))
	{
		BlockAttack(DamageCauser, AttackDamagedInfo.BlockPushPowar);

		return;
	}




	// ================ 대미지를 입는다 =========================


	// 스턴 여부
	FVector TargetToMeDir = GetActorLocation() - CauserPos;
	TargetToMeDir.Normalize();

	if (!AttackDamagedInfo.bIsDown)
	{
		if(AttackDamagedInfo.bSetStunTimeToHitAnim || !FMath::IsNearlyEqual(AttackDamagedInfo.StunTime, 0.f, 0.1f))
			Stun(AttackDamagedInfo.StunTime, AttackDamagedInfo.bSetStunTimeToHitAnim);
		
		AddImpulse(TargetToMeDir, AttackDamagedInfo.HitPushPowar);
	}
	else
	{
		KnockDown(AttackDamagedInfo.StunTime);
		AddImpulse(TargetToMeDir, AttackDamagedInfo.KnockDownPushPowar);

		SmoothTurnByCallOnce(CauserPos, 10.f);
	}

	// GP 여부
	DamagedByGPAttacked(AttackDamagedInfo.GPValue);

	// 스탯
	// StatComponent->DecreaseHP(AttackDamagedInfo.Damage);
	FS_DamageInfo DamageInfo;
	DamageInfo.Amount = AttackDamagedInfo.Damage;
	DamageInfo.DamageType = EDamageType::Melee;
	DamageInfo.bCanBeBlocked = false;
	DamageInfo.bCanBeParried = false;
	DamageInfo.bShouldForceInterrupt = false;
	DamageInfo.bShouldDamageInvincible = false;
	StatComponent->TakeDamage(DamageInfo, DamageCauser);

}

void ADK_Creature::DamagedByGPAttacked(int32 GPValue)
{
	// 가상 함수
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

	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), DodgeMontage, 1.f, 0.f);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ADK_Creature::EndDoge);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ADK_Creature::EndDoge);
	PlayMontageCallbackProxy->OnNotifyBegin.AddDynamic(this, &ADK_Creature::BeginNotifyDoge);
	PlayMontageCallbackProxy->OnNotifyEnd.AddDynamic(this, &ADK_Creature::EndNotifyDoge);

}

void ADK_Creature::EndDoge(FName NotifyName)
{
	bIsDodge = false;
	EndDodgeSkip_Notify();
}

void ADK_Creature::BeginNotifyDoge(FName NotifyName)
{
	if (NotifyName == FName("SkipDodge"))
	{
		BeginDodgeSkip_Notify();
	}

}

void ADK_Creature::EndNotifyDoge(FName NotifyName)
{
	if (NotifyName == FName("SkipDodge"))
	{
		EndDodgeSkip_Notify();
	}

}

void ADK_Creature::BeginDodgeSkip_Notify()
{
	bCanDodgeSkip = true;
}

void ADK_Creature::EndDodgeSkip_Notify()
{
	bCanDodgeSkip = false;
}

void ADK_Creature::PerfectDodge()
{
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

void ADK_Creature::BeBlockedPerfectly(int32 GPValue)
{

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
	
	//EndDodgeSkip_Notify();

}

void ADK_Creature::ResetInfoOnStun()
{
	//EndColRange_Notify(); // 공격 콜라이더
	//EndAttackRange_Notify(); // 공격 애니메이션 범위
	// ComboComponent->ResetComboInfo(); // 콤보

	//EndDodgeSkip_Notify(); // 무적시간 (혹시 모르니)

	StopSmoothTurn();

	EndBlock();
}

void ADK_Creature::ResetInfoOnKnockDown()
{
	ResetInfoOnStun();

	EndStun();

}

void ADK_Creature::ResetInfoOnDodge()
{
	// 공격 중에는 블락을 할 수 없지만, 혹시 모르니 
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	EndBlock();

}

void ADK_Creature::ResetInfoOnBlock()
{
	// 공격 중에는 회피를 할 수 없지만, 혹시 모르니 
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	EndDodgeSkip_Notify();

}

void ADK_Creature::ResetInfoOnHitBlock()
{
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	//EndDodgeSkip_Notify();

	StopSmoothTurn();
}






void ADK_Creature::SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy* Proxy)
{
	if (IsValid(PlayMontageCallbackProxy))
		PlayMontageCallbackProxy->OnInterrupted.Broadcast(NAME_None);

	PlayMontageCallbackProxy = Proxy;
}

void ADK_Creature::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnOffHUDHPBar(false);


}