// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/DK_Object.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Interface/DK_EnemyAIinterface.h"
#include "DK_Creature.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Creature: public ADK_Object, public IDK_EnemyAIInterface
{
	GENERATED_BODY()

public:
	ADK_Creature();


protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	// Common Section
public:
	FORCEINLINE void SetIsStrafe(bool bValue) { bIsStrafe = bValue; }
	FORCEINLINE bool GetIsStrafe() { return bIsStrafe; }

protected:
	void StartGroggy();
	
	virtual void DamagedByGPAttacked(int32 GPValue) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float IdleSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float WalkingSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float JoggingSpeed = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Speed)
	float SprintingSpeed = 0.f;

	bool bIsStrafe = false;





	// AI Section
public:
	FORCEINLINE UBehaviorTree* GetBTAsset() { return BTAsset; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init)
	TObjectPtr<UBehaviorTree> BTAsset;

	TObjectPtr<ADK_AIControllerBase> AIControllerBase;



	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UDK_CreatureStatComponent> CreatureStatComponent;

	UPROPERTY(EditAnywhere, Category = Stat)
	int32 RecoveryGPPerSec = 1.f;

	
	// Attack Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Attack)
	TObjectPtr<class UDK_AttackComponent> AttackComponent;


	// Block Section
public:
	virtual void BeBlockedPerfectly(int32 GPValue) override;


	// Stun
public:
	virtual void SetIsKnockDown(bool bValue) override;

	
	// Condition Section
protected:
	virtual bool CanKnockDown() override;
	

	// IDK_EnemyAIInterface
public:
	virtual void SetStrafe(bool bValue) override;
	virtual float SetMovementSpeed(EMovementSpeed MovementSpeed) override;

};
