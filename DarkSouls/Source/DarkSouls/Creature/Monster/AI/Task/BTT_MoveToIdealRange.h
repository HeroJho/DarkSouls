// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTT_MoveToIdealRange.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_MoveToIdealRange : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
//protected:
//	UFUNCTION()
//	void OnMoveFailed();
//	UFUNCTION()
//	void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);


protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector AttackTargetKey;

	UPROPERTY(EditAnywhere)
	float IdealRange = 0.f;

	TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompTree;

};
