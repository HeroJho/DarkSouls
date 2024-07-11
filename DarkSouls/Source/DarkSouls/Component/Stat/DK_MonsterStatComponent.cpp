// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_MonsterStatComponent.h"

#include "StatData/DK_MonsterStatDataAsset.h"




UDK_MonsterStatComponent::UDK_MonsterStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDK_MonsterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecoveringGP)
	{
		RecoveryGPTick(DeltaTime);
	}

}



void UDK_MonsterStatComponent::ResetStat(int32 CurRecoveryGPPerSpeed)
{
	Super::ResetStat();

	SetRecoveryGPPerSecSpeed(CurRecoveryGPPerSpeed);

	UDK_MonsterStatDataAsset* MonsterStatData = Cast<UDK_MonsterStatDataAsset>(StatData);

	MaxGP = MonsterStatData->MaxGP;
	DecreaseGP(0);
}

void UDK_MonsterStatComponent::BroadcastStat()
{
	Super::BroadcastStat();

}


void UDK_MonsterStatComponent::IncreaseGP(int32 Value)
{
	CurGP = FMath::Clamp(CurGP + Value, 0, MaxGP);

	if (CurGP >= MaxGP)
	{
		CurGP = 0;
		OnMaxGPDelegate.Broadcast();
	}

	OnChangeGPDelegate.Broadcast(CurGP, MaxGP);
}

void UDK_MonsterStatComponent::DecreaseGP(int32 Value)
{
	CurGP = FMath::Clamp(CurGP - Value, 0, MaxGP);

	OnChangeGPDelegate.Broadcast(CurGP, MaxGP);
}



void UDK_MonsterStatComponent::RecoveryGPTick(float DeltaTime)
{
	if (CurGP <= 0)
	{
		CurGP = 0;
		return;
	}


	RecoveryGPTimeAcc += DeltaTime;
	if (1.f < RecoveryGPTimeAcc)
	{
		DecreaseGP(CurRecoveryGPPerSec);
		RecoveryGPTimeAcc = 0.f;
	}

}
