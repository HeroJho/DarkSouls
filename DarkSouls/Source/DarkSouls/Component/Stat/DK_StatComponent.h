// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_StatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeHPDelegate, uint32, uint32);
DECLARE_MULTICAST_DELEGATE(FOnZeroHPDelegate);


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
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



public:
	virtual void ResetStat();

	FORCEINLINE uint32 GetMaxHP() { return MaxHP; }
	FORCEINLINE uint32 GetCurHP() { return CurHP; }


	void AddChangeHPDelegateFunc(UObject* Object, FName FuncName);
	void AddZeroHPDelegateFunc(UObject* Object, FName FuncName);

	UFUNCTION(BlueprintCallable)
	void IncreaseHP(int Value);
	UFUNCTION(BlueprintCallable)
	void DecreaseHP(int Value);



protected:
	FOnChangeHPDelegate Delegate_ChangeHP;
	FOnZeroHPDelegate Delegate_ZeroHP;


	int32 MaxHP = 100.f;
	int32 CurHP = 0.f;


};
