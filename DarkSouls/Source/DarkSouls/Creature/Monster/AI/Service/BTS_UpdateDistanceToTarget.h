// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdateDistanceToTarget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTS_UpdateDistanceToTarget : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_UpdateDistanceToTarget();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector DistanceToTargetKey;

};
