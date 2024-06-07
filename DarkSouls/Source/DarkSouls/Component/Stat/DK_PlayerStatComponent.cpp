// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_PlayerStatComponent.h"

UDK_PlayerStatComponent::UDK_PlayerStatComponent()
{
}

void UDK_PlayerStatComponent::ResetStat()
{
	Super::ResetStat();

	IncreaseSP(MaxSP);

}

void UDK_PlayerStatComponent::AddChangeSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeSP.AddUFunction(Object, FuncName);
}

void UDK_PlayerStatComponent::AddZeroSPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ZeroSP.AddUFunction(Object, FuncName);
}


void UDK_PlayerStatComponent::IncreaseSP(int Value)
{
	CurSP = FMath::Clamp(CurSP + Value, 0, MaxSP);

	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
}

void UDK_PlayerStatComponent::DecreaseSP(int Value)
{
	CurSP = FMath::Clamp(CurSP - Value, 0, MaxSP);


	if (CurSP == 0)
	{
		// ªÁ∏¡ µ®∏Æ∞‘¿Ã∆Æ

		Delegate_ZeroSP.Broadcast();
	}


	Delegate_ChangeSP.Broadcast(CurSP, MaxSP);
}
