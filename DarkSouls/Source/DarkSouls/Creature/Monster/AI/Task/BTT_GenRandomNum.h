// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_GenRandomNum.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_GenRandomNum : public UBTTaskNode
{
	GENERATED_BODY()
	

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector GenedNumber;

	UPROPERTY(EditAnywhere)
	float Min;
	UPROPERTY(EditAnywhere)
	float Max;

};
