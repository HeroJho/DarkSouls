// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsWithinIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTD_IsWithinIdealRange : public UBTDecorator
{
	GENERATED_BODY()
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	float IdealRange = 0.f;
	UPROPERTY(EditAnywhere)
	float ErrorMargin = 0.f;

};
