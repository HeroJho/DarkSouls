// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Enum/E_AIEnum.h"
#include "DK_AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_AIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	ADK_AIControllerBase();


	// Common Section
protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn);

protected:
	void RunAI(APawn* InPawn);
	void StopAI();

	// AI Section
protected:
	EAIState GetCurrentState();

	void SetStateAsPassive();
	void SetStateAsAttacking(AActor* AttackTarget, bool bUseLastKnownAttack);
	void SetStateAsInvestigating(FVector Location);
	void SetStateAsSeeking(FVector Location);
	void SetStateAsFrozen();
	void SetStateAsDead();

	// Perception Section
protected:
	UFUNCTION()
	void OnPerceptionUpdated_Notify(const TArray<AActor*>& UpdatedActors);
	UFUNCTION()
	void OnPerceptionTargetForgotten_Notify(AActor* Actor);

	bool CanSenseActor(FAIStimulus& OUT_Stimulus, AActor* Actor, EAISense Sense);

	void HandleSensedSight(AActor* Actor);
	void HandleLostSight(AActor* Actor);
	void HandleSensedSound(FVector Location);
	void HandleSensedDamage(AActor* Actor);
	void HandleForgetTarget(AActor* Actor);

	void SetTimerLosingSight();
	UFUNCTION()
	void SeekTarget();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent;

	FTimerHandle SeekAttackTargetTimerHandle;
	float TimetoSeekAfterLosingSight;

	// BBKeyName
protected:
	static const FName AIStateKey;
	static const FName AttackTargetKey;
	static const FName LocationOfInterestKey;

};
