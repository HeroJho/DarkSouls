// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combo/DK_ComboComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "PlayMontageCallbackProxy.h"

#include "Creature/DK_Creature.h"
#include "Component/Combo/DK_ComboActionData.h"



// Sets default values for this component's properties
UDK_ComboComponent::UDK_ComboComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void UDK_ComboComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<ADK_Creature>(GetOwner());
	
	ChangeComboActionData(0);
}


void UDK_ComboComponent::ProcessComboCommand()
{
	// 예약 상태라면 기존 재생하던 섹션만 재생한다
	if (ReserveComboActionDataIndex != -1)
	{
		return;
	}

	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
	else
	{
		bHasNextComboCommand = true;
	}

}

void UDK_ComboComponent::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;
	bHasNextComboCommand = false;

	UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurData->MontageSectionNamePrefix, CurrentCombo);


	Owner->SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		Owner->GetMesh(), CurData->ComboActionMontage, 1.f, 0.f, NextSection));
	
	BindEventFunc();
}

void UDK_ComboComponent::ComboCheck()
{
	if (bHasNextComboCommand)
	{
		bHasNextComboCommand = false;

		UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *CurData->MontageSectionNamePrefix, CurrentCombo);


		Owner->SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
			Owner->GetMesh(), CurData->ComboActionMontage, 1.f, 0.f, NextSection));

		BindEventFunc();
	}


}


void UDK_ComboComponent::BindEventFunc()
{
	UPlayMontageCallbackProxy* Proxy = Owner->GetMontageCallbackProxy();

	Proxy->OnCompleted.AddDynamic(this, &UDK_ComboComponent::ComboActionEnd);
	Proxy->OnInterrupted.AddDynamic(this, &UDK_ComboComponent::ComboActionInterrupted);
	Proxy->OnInterrupted.AddDynamic(Owner, &ADK_Creature::InterruptedAttack);

}

void UDK_ComboComponent::ComboActionInterrupted(FName NotifyName)
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	UAnimMontage* PlayMontage = AnimInstance->GetCurrentActiveMontage();

	UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];
	
	// 다른 몽타주로 Interrupte 됐다면 스탑
	if (PlayMontage != CurData->ComboActionMontage)
	{
		CurrentCombo = 0;
		bHasNextComboCommand = false;

		if (ReserveComboActionDataIndex != -1)
		{
			CurComboActionDataIndex = ReserveComboActionDataIndex;
			ReserveComboActionDataIndex = -1;
		}

	}

}

void UDK_ComboComponent::ComboActionEnd(FName NotifyName)
{
	CurrentCombo = 0;
	bHasNextComboCommand = false;

	if (ReserveComboActionDataIndex != -1)
	{
		CurComboActionDataIndex = ReserveComboActionDataIndex;
		ReserveComboActionDataIndex = -1;
	}

}



void UDK_ComboComponent::ChangeComboActionData(uint8 DataIndex)
{
	CurrentCombo = 0;
	bHasNextComboCommand = false;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		// 몽타주가 실행중인데, 콤보를 바꾼다? -> 기존꺼 끝내고 바꾼다
		if (AnimInstance->IsAnyMontagePlaying())
		{
			ReserveComboActionDataIndex = DataIndex;
		}
		else
		{
			CurComboActionDataIndex = DataIndex;
			ReserveComboActionDataIndex = -1;
		}
	}

}

const TArray<FString>& UDK_ComboComponent::GetCurrentAttackCollisionInfos()
{
	return ComboActionDatas[CurComboActionDataIndex]->AttackColInfos[CurrentCombo].AttackCollisions;
}

FAttackDamagedInfo UDK_ComboComponent::GetCurrentAttackInfos()
{
	const FAttackInfo& AttackInfo = ComboActionDatas[CurComboActionDataIndex]->AttackColInfos[CurrentCombo];

	FAttackDamagedInfo Result;

	Result.Damage = AttackInfo.Damage;
	Result.GPValue = AttackInfo.GPValue;
	Result.bIsDown = AttackInfo.bIsDown;
	Result.bSetStunTimeToHitAnim = AttackInfo.bSetStunTimeToHitAnim;
	Result.StunTime = AttackInfo.StunTime;

	Result.HitPushPowar = AttackInfo.HitPushPowar;
	Result.KnockDownPushPowar = AttackInfo.KnockDownPushPowar;
	Result.BlockPushPowar = AttackInfo.BlockPushPowar;

	return Result;
}

void UDK_ComboComponent::ResetComboInfo()
{
	// 이 인덱스에 따라 콤보가 실행됨
	CurrentCombo = 0;
	bHasNextComboCommand = false;

	ReserveComboActionDataIndex = -1;
}
