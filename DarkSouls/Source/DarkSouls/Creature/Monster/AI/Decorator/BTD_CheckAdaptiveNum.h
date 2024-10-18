// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckAdaptiveNum.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTD_CheckAdaptiveNum : public UBTDecorator
{
	GENERATED_BODY()

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector GenedNumKey;

	UPROPERTY(EditAnywhere)
	int TableIndex = -1;
	UPROPERTY(EditAnywhere)
	int Num = -1;

};
