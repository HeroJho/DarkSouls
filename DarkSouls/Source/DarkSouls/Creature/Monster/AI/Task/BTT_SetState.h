// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enum/E_AIEnum.h"
#include "BTT_SetState.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_SetState : public UBTTaskNode
{
	GENERATED_BODY()
	

protected:
	// Use Only Passive
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;


protected:
	UPROPERTY(EditAnywhere)
	EAIState State = EAIState::Defualt;

};
