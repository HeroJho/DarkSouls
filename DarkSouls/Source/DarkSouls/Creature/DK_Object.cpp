// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Object.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayMontageCallbackProxy.h"

#include "Tool/Define.h"
#include "Game/DK_GameMode.h"
#include "Manager/DK_UIManager.h"
#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Component/Stat/DK_StatComponent.h"

#include "UI/DK_WidgetComponent.h"
#include "UI/DK_HUDWidget.h"
#include "UI/DK_SmoothBarWidget.h"


// Sets default values
ADK_Object::ADK_Object()
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


	// MiddlePosArrowComponent
	MiddlePosArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("MiddlePosArrowComponent"));
	MiddlePosArrowComponent->SetupAttachment(RootComponent);
}

void ADK_Object::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	StatComponent->OnDamageResponseDelegate.AddUObject(this, &ADK_Object::OnHitReaction_Notify);
	StatComponent->OnBlockDelegate.AddUObject(this, &ADK_Object::OnBlock_Notify);
	StatComponent->OnDodgSkipDelegate.AddUObject(this, &ADK_Object::OnDodgeSkip_Notify);

	CollisionManagerComponent->OnColHitDelegate.AddUObject(this, &ADK_Object::OnColHit_Notify);

	ComboComponent->OnComboInterruptedForTaskDelegate.AddUObject(this, &ADK_Object::EndAttack_Notify);
	ComboComponent->OnComboEndForTaskDelegate.AddUObject(this, &ADK_Object::EndAttack_Notify);
}

// Called when the game starts or when spawned
void ADK_Object::BeginPlay()
{
	Super::BeginPlay();

	OnOffHUDHPBar(false);
	OnOffScreenHPBar(false);

}

// Called every frame
void ADK_Object::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 타이머로 할려고했는데, 카메라가 흔들리는 문제가 생김
	if (bIsSmoothTurn)
	{
		SmoothTurnTick();
	}


}





void ADK_Object::AddImpulse(FVector Dir, float Powar)
{
	Dir.Normalize();
	GetCharacterMovement()->AddImpulse(Dir * Powar, true);
	LaunchCharacter(Dir * Powar, true, true);
}




void ADK_Object::OnOffScreenHPBar(bool bIsOn)
{
	WidgetComponent->SetHiddenInGame(!bIsOn);
}

void ADK_Object::OnOffHUDHPBar(bool bIsOn)
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
		BossHpBarDelegateHandle = StatComponent->OnChangeHPDelegate.AddUFunction(BossSmoothBarWidget.Get(), FName("UpdateBar"));

		StatComponent->BroadcastStat();

		
	}
	else
	{
		HUDWidget->RemoveBossHpBar(BossSmoothBarWidget.Get());
		StatComponent->OnChangeHPDelegate.Remove(BossHpBarDelegateHandle);
		
		BossHpBarDelegateHandle.Reset();
		BossSmoothBarWidget = nullptr;
	}
	
}




void ADK_Object::BeginAttackRange_Notify()
{
	bIsInAttackRange = true;
}

void ADK_Object::EndAttackRange_Notify()
{
	bIsInAttackRange = false;
	CollisionManagerComponent->ClearActorTemps();
}

void ADK_Object::BeginColRange_Notify()
{
	const TArray<FString>& AttackCollisionInfos = ComboComponent->GetCurrentAttackCollisionInfos();
	CollisionManagerComponent->TurnAttackCol(AttackCollisionInfos, true);

}

void ADK_Object::EndColRange_Notify()
{
	CollisionManagerComponent->TurnBlockAllCol();

}


void ADK_Object::InterruptedAttack_Notify()
{
	EndAttackRange_Notify();
	EndColRange_Notify();
}

void ADK_Object::EndAttack_Notify()
{
	bIsAttacking = false;
}



void ADK_Object::OnColHit_Notify(IDK_DamageableInterface* HitActor)
{
	FS_DamageInfo DamageInfo = ComboComponent->GetCurrentAttackInfos();
	
	HitActor->TakeDamage(DamageInfo, this);
}

void ADK_Object::DamagedByGPAttacked(int32 GPValue)
{
	// 가상 함수
}







void ADK_Object::Stun(float StunTime, bool bSetAnimTime)
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
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ADK_Object::EndStun, StunTime, false);
	
	if (HitMontage)
	{
		PlayAnimMontage(HitMontage, 1.f);
	}
	
}


void ADK_Object::EndStun()
{
	bIsStun = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(HitMontage))
		StopAnimMontage(HitMontage);
}






void ADK_Object::SetIsKnockDown(bool bValue)
{
	bIsKnockDown = bValue;
}


void ADK_Object::KnockDown(float KnockDownTime)
{
	if (!CanKnockDown())
		return;

	ResetInfoOnKnockDown();

	GetWorldTimerManager().ClearTimer(KnockDownTimerHandle);
	GetWorldTimerManager().ClearTimer(EndKnockDownTimerHandle);

	GetWorldTimerManager().SetTimer(KnockDownTimerHandle, this, &ADK_Object::StartEndKnockDown, KnockDownTime, false);

	if (StartKnockDownMontage)
	{
		SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
			GetMesh(), StartKnockDownMontage, 1.f, 0.f));
	}
	
	SetIsKnockDown(true);
}


void ADK_Object::StartEndKnockDown()
{
	if (EndKnockDownMontage)
	{
		SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
			GetMesh(), EndKnockDownMontage, 1.f, 0.f));

		PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ADK_Object::EndKnockDown);


		const float DisPlayTime = 0.25f;
		float PlayTime = EndKnockDownMontage->GetPlayLength();
		
		GetWorldTimerManager().ClearTimer(KnockDownTimerHandle);
		GetWorldTimerManager().ClearTimer(EndKnockDownTimerHandle);

		FTimerDelegate KnockDownTimer;
		KnockDownTimer.BindUFunction(this, FName("EndKnockDown"), FName());
		GetWorldTimerManager().SetTimer(EndKnockDownTimerHandle, KnockDownTimer, PlayTime - DisPlayTime, false);
		
	}
	else
	{
		SetIsKnockDown(false);
	}
}

void ADK_Object::EndKnockDown(FName NotifyName)
{
	GetWorldTimerManager().ClearTimer(EndKnockDownTimerHandle);
	SetIsKnockDown(false);
}





void ADK_Object::SmallHittedTrigger()
{
	bSmallHittedTrigger = true;

	GetWorldTimerManager().ClearTimer(SmallHittedTimerHandle);
	GetWorldTimerManager().SetTimer(SmallHittedTimerHandle, FTimerDelegate::CreateLambda(
		[this]() { bSmallHittedTrigger = false; }), 0.05f, false);
}



void ADK_Object::SmoothTurnByCallOnce(FVector InDestPos, float TurnSpeed)
{
	if (!CanSmoothTurn())
		return;

	DestPos = InDestPos;
	SmoothTurnSpeed = TurnSpeed;

	bIsSmoothTurn = true;
}

void ADK_Object::StopSmoothTurn()
{
	bIsSmoothTurn = false;
}

void ADK_Object::SmoothTurnTick()
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






void ADK_Object::Dodge()
{
	if (!CanDodge())
		return;

	ResetInfoOnDodge();

	bIsDodge = true;

	PlayMontageCallbackProxy = UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(GetMesh(), DodgeMontage, 1.f, 0.f);
	PlayMontageCallbackProxy->OnCompleted.AddDynamic(this, &ADK_Object::EndDoge);
	PlayMontageCallbackProxy->OnInterrupted.AddDynamic(this, &ADK_Object::EndDoge);
	PlayMontageCallbackProxy->OnNotifyBegin.AddDynamic(this, &ADK_Object::BeginNotifyDoge);
	PlayMontageCallbackProxy->OnNotifyEnd.AddDynamic(this, &ADK_Object::EndNotifyDoge);

}

void ADK_Object::EndDoge(FName NotifyName)
{
	bIsDodge = false;
	EndDodgeSkip_Notify();
}

void ADK_Object::BeginNotifyDoge(FName NotifyName)
{
	if (NotifyName == FName("SkipDodge"))
	{
		BeginDodgeSkip_Notify();
	}

}

void ADK_Object::EndNotifyDoge(FName NotifyName)
{
	if (NotifyName == FName("SkipDodge"))
	{
		EndDodgeSkip_Notify();
	}

}

void ADK_Object::BeginDodgeSkip_Notify()
{
	StatComponent->SetIsDodgeSkip(true);
}

void ADK_Object::EndDodgeSkip_Notify()
{
	StatComponent->SetIsDodgeSkip(false);
}









void ADK_Object::BlockAttack(bool bCanParrying, float KnockBackPowar, AActor* DamageCauser)
{
	HitBlock();
	AddImpulse(GetActorForwardVector() * -1.f, KnockBackPowar);

}

bool ADK_Object::IsBlock()
{
	return StatComponent->GetIsBlocking();
}

void ADK_Object::Block()
{
	if (!CanBlock())
		return;

	ResetInfoOnBlock();

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = BlockSpeed;

	// TODO: 몽타주로 관리하는게 아니라서 false가 안 될 경우가 있는지 확인 필요!
	// 모든 인터럽트에 EndBlock 호출해 주기
	StatComponent->SetIsBlocking(true);
	
}

void ADK_Object::EndBlock()
{
	// GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	StopSmoothTurn();

	StatComponent->SetIsBlocking(false);

}



void ADK_Object::HitBlock()
{
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
	GetWorldTimerManager().SetTimer(HitBlockTimerHandle, this, &ADK_Object::EndHitBlock, HitDelayTime, false);


}

void ADK_Object::EndHitBlock()
{
	bIsHitBlock = false;

}

void ADK_Object::BeBlockedPerfectly(int32 GPValue)
{

}






FVector ADK_Object::GetMiddlePos()
{
	return MiddlePosArrowComponent->GetComponentLocation();
}





bool ADK_Object::CanAttack()
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

bool ADK_Object::CanDodge()
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

bool ADK_Object::CanSmoothTurn()
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

bool ADK_Object::CanStun()
{
	if (bIsKnockDown)
		return false;


	return true;
}

bool ADK_Object::CanKnockDown()
{
	if (bIsKnockDown)
		return false;

	return true;
}

bool ADK_Object::CanBlock()
{
	if (bIsDodge)
		return false;

	if (bIsStun)
		return false;

	if (bIsKnockDown)
		return false;


	return true;
}





void ADK_Object::ResetInfoOnAttack()
{
	EndBlock();
	
	//EndDodgeSkip_Notify();

}

void ADK_Object::ResetInfoOnStun()
{
	//EndColRange_Notify(); // 공격 콜라이더
	//EndAttackRange_Notify(); // 공격 애니메이션 범위
	// ComboComponent->ResetComboInfo(); // 콤보

	//EndDodgeSkip_Notify(); // 무적시간 (혹시 모르니)

	StopSmoothTurn();

	EndBlock();
}

void ADK_Object::ResetInfoOnKnockDown()
{
	ResetInfoOnStun();

	EndStun();

}

void ADK_Object::ResetInfoOnDodge()
{
	// 공격 중에는 블락을 할 수 없지만, 혹시 모르니 
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	EndBlock();

}

void ADK_Object::ResetInfoOnBlock()
{
	// 공격 중에는 회피를 할 수 없지만, 혹시 모르니 
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	EndDodgeSkip_Notify();

}

void ADK_Object::ResetInfoOnHitBlock()
{
	//EndColRange_Notify();
	//EndAttackRange_Notify();
	//ComboComponent->ResetComboInfo();

	//EndDodgeSkip_Notify();

	StopSmoothTurn();
}






void ADK_Object::SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy* Proxy)
{
	/*if (IsValid(PlayMontageCallbackProxy))
		PlayMontageCallbackProxy->OnInterrupted.Broadcast(NAME_None);*/

	PlayMontageCallbackProxy = Proxy;
}

void ADK_Object::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	OnOffHUDHPBar(false);


}









void ADK_Object::OnHitReaction_Notify(EDamageResponse DamageResponseType, int32 GPValue, float StunTime, float KnockBackPowar, AActor* DamageCauser)
{
	// 그로기 수치 
	DamagedByGPAttacked(GPValue);

	bool bIsAnimStunTime = false;
	if (StunTime <= 0.f)
	{
		bIsAnimStunTime = true;
	}

	FVector TargetToMeDir = GetActorLocation() - DamageCauser->GetActorLocation();
	TargetToMeDir.Normalize();

	switch (DamageResponseType)
	{
	case EDamageResponse::None:
		break;
	case EDamageResponse::HitReaction:
		SmallHittedTrigger();
		AddImpulse(TargetToMeDir, KnockBackPowar);
		break;
	case EDamageResponse::Stagger:
		break;
	case EDamageResponse::Stun:
		Stun(3.f, bIsAnimStunTime);
		AddImpulse(TargetToMeDir, KnockBackPowar);
		break;
	case EDamageResponse::KnockBack:
		KnockDown(StunTime);
		AddImpulse(TargetToMeDir, KnockBackPowar);
		break;
	default:
		break;
	}

}

void ADK_Object::OnBlock_Notify(bool bCanParrying, float KnockBackPowar, AActor* DamageCauser)
{
	BlockAttack(bCanParrying, KnockBackPowar, DamageCauser);

}

void ADK_Object::OnDodgeSkip_Notify()
{
	// 가상함수
}



bool ADK_Object::TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser)
{
	return StatComponent->TakeDamage(DamageInfo, DamageCauser);
}

bool ADK_Object::IsDead()
{
	return StatComponent->GetIsDead();
}

