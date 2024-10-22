// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_IsAttacked.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTD_IsAttacked : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTD_IsAttacked();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
