// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CanSeeTarget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTD_CanSeeTarget : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_CanSeeTarget();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector TargetKey;

};
