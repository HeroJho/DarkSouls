// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/DK_StatComponent.h"
#include "DK_MonsterStatComponent.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_MonsterStatComponent : public UDK_StatComponent
{
	GENERATED_BODY()
	
public:
	UDK_MonsterStatComponent();


public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;




public:
	void ResetStat(int32 CurRecoveryGPPerSpeed);

	virtual void BroadcastStat() override;

	FDelegateHandle AddChangeGPDelegateFunc(UObject* Object, FName FuncName);
	void AddMaxGPDelegateFunc(UObject* Object, FName FuncName);

	void IncreaseGP(int32 Value);
	void DecreaseGP(int32 Value);

	FORCEINLINE void SetRecoveryGPPerSecSpeed(int32 Speed) { CurRecoveryGPPerSec = Speed; }


protected:
	void RecoveryGPTick(float DeltaTime);

	FORCEINLINE void StartRecoveryGP() { bIsRecoveringGP = true; }


protected:
	FOnChangeDelegate Delegate_ChangeGP;
	FOnZeroDelegate Delegate_MaxGP;

	int32 MaxGP = 400;
	int32 CurGP = 0;

	int32 CurRecoveryGPPerSec = 0.f;

	bool bIsRecoveringGP = false;
	FTimerHandle RecoveryGPTimerHandle;
	float RecoveryGPTimeAcc = 0.f;


};
