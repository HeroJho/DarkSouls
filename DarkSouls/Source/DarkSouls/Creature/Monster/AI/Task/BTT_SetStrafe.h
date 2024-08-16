// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_SetStrafe.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_SetStrafe : public UBTTaskNode
{
	GENERATED_BODY()
	

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
	UPROPERTY(EditAnywhere)
	uint8 bIsStrafe : 1;


};
