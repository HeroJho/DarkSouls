// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/DK_StatComponent.h"
#include "DK_PlayerStatComponent.generated.h"



UCLASS()
class DARKSOULS_API UDK_PlayerStatComponent : public UDK_StatComponent
{
	GENERATED_BODY()
	
public:
	UDK_PlayerStatComponent();


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	void ResetStat(int32 RecoverySPSpeed, int32 DecoveryTPSpeed);

	virtual void BroadcastStat() override;


	// Adj Max Value
public:
	UFUNCTION(BlueprintCallable)
	void ChangeMaxHP(int32 Value);
	UFUNCTION(BlueprintCallable)
	void ChangeMaxSP(int32 Value);
	
	void AddChangeMaxHPDelegateFunc(UObject* Object, FName FuncName);
	void AddChangeMaxSPDelegateFunc(UObject* Object, FName FuncName);

protected:
	FOnChangeDelegate Delegate_ChangeMaxHP;
	FOnChangeDelegate Delegate_ChangeMaxSP;





	// SP Section
public:
	FORCEINLINE void SetRecoverySPPerSecSpeed(int32 Speed) { CurRecoverySPPerSec = Speed; }

	void AddChangeSPDelegateFunc(UObject* Object, FName FuncName);	

	void IncreaseSP(int32 Value);
	void DecreaseSP(int32 Value);

	FORCEINLINE bool IsZeroSP();


	void DelayRecoverySP(float DelaySec);
	bool CanUse(int32 MinValue);

	
protected:
	void RecoverySPTick(float DeltaTime);
	
	FORCEINLINE void StartRecoverySP() { bIsRecoveringSP = true; }
	FORCEINLINE void StopRecoverySP() { bIsRecoveringSP = false; RecoverySPTimeAcc = 0.f; }



protected:
	FOnChangeDelegate Delegate_ChangeSP;

	int32 MaxSP = 400;
	int32 CurSP = 0;

	int32 CurRecoverySPPerSec = 0;

	bool bIsRecoveringSP = false;
	FTimerHandle RecoverySPTimerHandle;
	float RecoverySPTimeAcc = 0.f;





	// TP Section
public:
	void IncreaseTP(int32 Value);

	void AddChangeTPDelegateFunc(UObject* Object, FName FuncName);
	void AddReleaseTPDelegateFunc(UObject* Object, FName FuncName);


protected:
	void DecoveryTPTick(float DeltaTime);

	void DecreaseTP(int32 Value);
	FORCEINLINE void SetDecreaseTPPerSecSpeed(int32 Value) { CurDecoveryTPPerSec = Value; }



protected:
	FOnChangeDelegate Delegate_ChangeTP;
	FOnZeroDelegate Delegate_ReleaseTP;

	int32 MaxTP = 0;
	int32 CurTP = 0;

	int32 CurDecoveryTPPerSec = 0;

	FTimerHandle DecoveryTPTimerHandle;
	float DecoveryTPTimeAcc = 0.f;


};
