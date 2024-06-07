// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/DK_StatComponent.h"
#include "DK_PlayerStatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeSPDelegate, uint32, uint32);
DECLARE_MULTICAST_DELEGATE(FOnZeroSPDelegate);


UCLASS()
class DARKSOULS_API UDK_PlayerStatComponent : public UDK_StatComponent
{
	GENERATED_BODY()
	
public:
	UDK_PlayerStatComponent();



public:
	virtual void ResetStat() override;

	FORCEINLINE uint32 GetMaxSP() { return MaxSP; }
	FORCEINLINE uint32 GetCurSP() { return CurSP; }

	void AddChangeSPDelegateFunc(UObject* Object, FName FuncName);
	void AddZeroSPDelegateFunc(UObject* Object, FName FuncName);

	UFUNCTION(BlueprintCallable)
	void IncreaseSP(int Value);
	UFUNCTION(BlueprintCallable)
	void DecreaseSP(int Value);


protected:
	FOnChangeSPDelegate Delegate_ChangeSP;
	FOnZeroSPDelegate Delegate_ZeroSP;

	int32 MaxSP = 100.f;
	int32 CurSP = 0.f;


};
