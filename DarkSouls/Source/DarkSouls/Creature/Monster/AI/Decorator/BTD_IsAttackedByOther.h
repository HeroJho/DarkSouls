// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTD_IsAttackedByOther.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTD_IsAttackedByOther : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()


public:
	UBTD_IsAttackedByOther();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};