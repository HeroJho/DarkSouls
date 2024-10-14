// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combo/DK_ComboComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "PlayMontageCallbackProxy.h"

#include "Creature/DK_Object.h"
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
	Owner = Cast<ADK_Object>(GetOwner());
	
	ChangeComboActionData(0);
}


void UDK_ComboComponent::ProcessComboCommand(bool InbIsAllProcess)
{
	bIsAllProcess = InbIsAllProcess;

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

void UDK_ComboComponent::ComboCheck_Notify()
{
	if (bIsAllProcess || bHasNextComboCommand)
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

void UDK_ComboComponent::PlayNextSection()
{
	bHasNextComboCommand = false;

	UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];

	CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurData->MaxComboCount);
	FName NextSection = *FString::Printf(TEXT("%s%d"), *CurData->MontageSectionNamePrefix, CurrentCombo);

	// * ComboCheck_Notify로 직접 호출할 경우 InterruptedComboAction 함수의 현재 실행중인 몽타주가 없다고 나옴.
	// 따라서 인터럽트 비워준다
	UPlayMontageCallbackProxy* Proxy = Owner->GetMontageCallbackProxy();
	Proxy->OnInterrupted.Clear();

	Owner->SetMontageCallbackProxyWithIntrrupted(UPlayMontageCallbackProxy::CreateProxyObjectForPlayMontage(
		Owner->GetMesh(), CurData->ComboActionMontage, 1.f, 0.f, NextSection));

	BindEventFunc();

}


void UDK_ComboComponent::BindEventFunc()
{
	// 크리쳐 함수 바인딩
	OnSectionEndDelegate.Broadcast();

	UPlayMontageCallbackProxy* Proxy = Owner->GetMontageCallbackProxy();

	Proxy->OnCompleted.AddDynamic(this, &UDK_ComboComponent::EndComboAction);
	Proxy->OnInterrupted.AddDynamic(this, &UDK_ComboComponent::InterruptedComboAction);
	Proxy->OnNotifyBegin.AddDynamic(this, &UDK_ComboComponent::BeginNotifyComboAction);
	Proxy->OnNotifyEnd.AddDynamic(this, &UDK_ComboComponent::EndNotifyComboAction);
}

void UDK_ComboComponent::BeginNotifyComboAction(FName NotifyName)
{
	if (NotifyName == FName("ComboCheck"))
	{
		ComboCheck_Notify();
	}

}

void UDK_ComboComponent::EndNotifyComboAction(FName NotifyName)
{

}

void UDK_ComboComponent::InterruptedComboAction(FName NotifyName)
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	UAnimMontage* PlayMontage = AnimInstance->GetCurrentActiveMontage();
	
	UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];
	

	// 다른 몽타주로 Interrupte 됐다면 스탑
	if (PlayMontage != CurData->ComboActionMontage)
	{
		CurrentCombo = 0;
		bHasNextComboCommand = false;
		bIsAllProcess = false;

		if (ReserveComboActionDataIndex != -1)
		{
			CurComboActionDataIndex = ReserveComboActionDataIndex;
			ReserveComboActionDataIndex = -1;
		}

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("InterruptedComboAction")));
		OnComboInterruptedDelegate.Broadcast();
	}

}

void UDK_ComboComponent::EndComboAction(FName NotifyName)
{
	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsAllProcess = false;

	if (ReserveComboActionDataIndex != -1)
	{
		CurComboActionDataIndex = ReserveComboActionDataIndex;
		ReserveComboActionDataIndex = -1;
	}

	OnComboEndDelegate.Broadcast();
}



void UDK_ComboComponent::ChangeComboActionData(uint8 DataIndex)
{
	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsAllProcess = false;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		// 콤보 중에, 콤보를 바꾸면
		if (Owner->IsAttacking())
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
	return ComboActionDatas[CurComboActionDataIndex]->DamageInfos[CurrentCombo].AttackColInfos;
}

FS_DamageInfo UDK_ComboComponent::GetCurrentAttackInfos()
{
	return ComboActionDatas[CurComboActionDataIndex]->DamageInfos[CurrentCombo];
}

void UDK_ComboComponent::ResetComboInfo()
{
	// 이 인덱스에 따라 콤보가 실행됨
	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsAllProcess = false;

	ReserveComboActionDataIndex = -1;
}
