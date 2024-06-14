// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/DK_Creature.h"
#include "DK_Monster.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Monster : public ADK_Creature
{
	GENERATED_BODY()

public:
	ADK_Monster();


protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	// Common Section
protected:
	UFUNCTION()
	void StartGroggy();
	
	virtual void DamagedByGPAttacked(int32 GPValue) override;



	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UDK_MonsterStatComponent> MonsterStatComponent;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 RecoveryGPPerSec = 1.f;


	// Block Section
public:
	virtual void BeBlockedPerfectly(int32 GPValue) override;


	
	// Condition Section
protected:
	virtual bool CanKnockDown() override;


};
