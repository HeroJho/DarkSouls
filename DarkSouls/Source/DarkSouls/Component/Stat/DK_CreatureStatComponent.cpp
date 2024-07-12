// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_CreatureStatComponent.h"

#include "StatData/DK_CreatureStatDataAsset.h"




UDK_CreatureStatComponent::UDK_CreatureStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDK_CreatureStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsRecoveringGP)
	{
		RecoveryGPTick(DeltaTime);
	}

}



void UDK_CreatureStatComponent::ResetStat(int32 CurRecoveryGPPerSpeed)
{
	Super::ResetStat();

	SetRecoveryGPPerSecSpeed(CurRecoveryGPPerSpeed);

	UDK_CreatureStatDataAsset* MonsterStatData = Cast<UDK_CreatureStatDataAsset>(StatData);

	MaxGP = MonsterStatData->MaxGP;
	DecreaseGP(0);
}

void UDK_CreatureStatComponent::BroadcastStat()
{
	Super::BroadcastStat();

}


void UDK_CreatureStatComponent::IncreaseGP(int32 Value)
{
	CurGP = FMath::Clamp(CurGP + Value, 0, MaxGP);

	if (CurGP >= MaxGP)
	{
		CurGP = 0;
		OnMaxGPDelegate.Broadcast();
	}

	OnChangeGPDelegate.Broadcast(CurGP, MaxGP);
}

void UDK_CreatureStatComponent::DecreaseGP(int32 Value)
{
	CurGP = FMath::Clamp(CurGP - Value, 0, MaxGP);

	OnChangeGPDelegate.Broadcast(CurGP, MaxGP);
}



void UDK_CreatureStatComponent::RecoveryGPTick(float DeltaTime)
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
