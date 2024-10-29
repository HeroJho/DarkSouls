// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Enum/E_MovementSpeed.h"
#include "BTT_SetMovementSpeed.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_SetMovementSpeed : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_SetMovementSpeed();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovementSpeed SpeedMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;


};
