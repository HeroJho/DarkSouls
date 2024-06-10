// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_StatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeDelegate, int32, int32);
DECLARE_MULTICAST_DELEGATE(FOnZeroDelegate);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_StatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	virtual void ResetStat();

	virtual void BroadcastStat();

	FDelegateHandle AddChangeHPDelegateFunc(UObject* Object, FName FuncName);
	void RemoveChangeHPDelegateFunc(FDelegateHandle Handle);

	void AddZeroHPDelegateFunc(UObject* Object, FName FuncName);


	void IncreaseHP(int32 Value);
	void DecreaseHP(int32 Value);



protected:
	FOnChangeDelegate Delegate_ChangeHP;
	FOnZeroDelegate Delegate_ZeroHP;


	int32 MaxHP = 400;
	int32 CurHP = 0;


};
