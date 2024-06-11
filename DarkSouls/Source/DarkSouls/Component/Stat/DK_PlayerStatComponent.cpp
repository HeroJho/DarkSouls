// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_PlayerStatComponent.h"

#include "StatData/DK_PlayerStatDataAsset.h"

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

	DecoveryTPTick(DeltaTime);

}



void UDK_PlayerStatComponent::ResetStat(int32 RecoverySPSpeed, int32 DecoveryTPSpeed)
{
	Super::ResetStat();

	SetRecoverySPPerSecSpeed(RecoverySPSpeed);
	SetDecreaseTPPerSecSpeed(DecoveryTPSpeed);

	bIsRecoveringSP = true;

	UDK_PlayerStatDataAsset* PlayerStatData = Cast<UDK_PlayerStatDataAsset>(StatData);

	MaxSP = PlayerStatData->MaxSP;
	MaxTP = PlayerStatData->MaxTP;

	IncreaseSP(MaxSP);
	DecreaseTP(0);

	// UI 크기 바꾸는 델리게이트 실행
	ChangeMaxHP(MaxHP); 
	ChangeMaxSP(MaxSP);
}

void UDK_PlayerStatComponent::BroadcastStat()
{
	Super::BroadcastStat();

	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
	Delegate_ChangeTP.Broadcast(CurTP, MaxTP);
}



void UDK_PlayerStatComponent::ChangeMaxHP(int32 Value)
{
	MaxHP = Value;
	if (MaxHP < CurHP)
		CurHP = MaxHP;

	Delegate_ChangeMaxHP.Broadcast(CurHP, MaxHP);
}

void UDK_PlayerStatComponent::ChangeMaxSP(int32 Value)
{
	MaxSP = Value;
	if (MaxSP < CurSP)
		CurSP = MaxSP;

	Delegate_ChangeMaxSP.Broadcast(CurSP, MaxSP);
}

void UDK_PlayerStatComponent::AddChangeMaxHPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeMaxHP.AddUFunction(Object, FuncName);
}

void UDK_PlayerStatComponent::AddChangeMaxSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeMaxSP.AddUFunction(Object, FuncName);
}





void UDK_PlayerStatComponent::AddChangeSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeSP.AddUFunction(Object, FuncName);
}


void UDK_PlayerStatComponent::IncreaseSP(int32 Value)
{
	CurSP = FMath::Clamp(CurSP + Value, 0, MaxSP);

	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
}

void UDK_PlayerStatComponent::DecreaseSP(int32 Value)
{
	CurSP = FMath::Clamp(CurSP - Value, 0, MaxSP);

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
	if (CurSP >= MaxSP)
	{
		CurSP = MaxSP;
		return;
	}


	RecoverySPTimeAcc += DeltaTime;
	if (1.f < RecoverySPTimeAcc)
	{
		IncreaseSP(CurRecoverySPPerSec);
		RecoverySPTimeAcc = 0.f;
	}

}

void UDK_PlayerStatComponent::IncreaseTP(int32 Value)
{
	CurTP = FMath::Clamp(CurTP + Value, 0, MaxTP);

	if (CurTP >= MaxTP)
	{
		CurTP = 0;
		Delegate_ReleaseTP.Broadcast();
	}

	Delegate_ChangeTP.Broadcast(CurTP, MaxTP);
}

void UDK_PlayerStatComponent::DecreaseTP(int32 Value)
{
	CurTP = FMath::Clamp(CurTP - Value, 0, MaxTP);

	Delegate_ChangeTP.Broadcast(CurTP, MaxTP);
}

void UDK_PlayerStatComponent::AddChangeTPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeTP.AddUFunction(Object, FuncName);
}

void UDK_PlayerStatComponent::AddReleaseTPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ReleaseTP.AddUFunction(Object, FuncName);
}

void UDK_PlayerStatComponent::DecoveryTPTick(float DeltaTime)
{
	if (CurTP <= 0)
	{
		CurTP = 0;
		return;
	}


	DecoveryTPTimeAcc += DeltaTime;
	if (1.f < DecoveryTPTimeAcc)
	{
		DecreaseTP(CurDecoveryTPPerSec);
		DecoveryTPTimeAcc = 0.f;
	}

}
