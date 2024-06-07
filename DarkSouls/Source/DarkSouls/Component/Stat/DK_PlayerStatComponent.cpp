// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_PlayerStatComponent.h"

UDK_PlayerStatComponent::UDK_PlayerStatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDK_PlayerStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (bIsRecoveringSP)
	{
		RecoverySPTick(DeltaTime);
	}

}

void UDK_PlayerStatComponent::ResetStat()
{
	Super::ResetStat();

	IncreaseSP(MaxSP);

	bIsRecoveringSP = true;
}

void UDK_PlayerStatComponent::AddChangeSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeSP.AddUFunction(Object, FuncName);
}

void UDK_PlayerStatComponent::AddZeroSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ZeroSP.AddUFunction(Object, FuncName);
}


void UDK_PlayerStatComponent::IncreaseSP(int32 Value)
{
	CurSP = FMath::Clamp(CurSP + Value, 0, MaxSP);

	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
}

void UDK_PlayerStatComponent::DecreaseSP(int32 Value)
{
	CurSP = FMath::Clamp(CurSP - Value, 0, MaxSP);


	if (CurSP == 0)
	{
		// »ç¸Á µ¨¸®°ÔÀÌÆ®

		Delegate_ZeroSP.Broadcast();
	}


	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
}

bool UDK_PlayerStatComponent::IsZeroSP()
{
	if (CurSP == 0)
		return true;

	return false;
}


void UDK_PlayerStatComponent::DelayRecoverySP(float DelaySec)
{
	StopRecoverySP();
	GetWorld()->GetTimerManager().ClearTimer(RecoverySPTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(RecoverySPTimerHandle, this, &UDK_PlayerStatComponent::StartRecoverySP, DelaySec, false);
}

bool UDK_PlayerStatComponent::CanUse(int32 MinValue)
{
	if (CurSP >= MinValue)
		return true;

	return false;
}


void UDK_PlayerStatComponent::RecoverySPTick(float DeltaTime)
{
	if (CurSP == MaxSP)
		return;

	RecoverySPTimeAcc += DeltaTime;
	if (1.f < RecoverySPTimeAcc)
	{
		IncreaseSP(RecoverySPPerSec);
		RecoverySPTimeAcc = 0.f;
	}

}
