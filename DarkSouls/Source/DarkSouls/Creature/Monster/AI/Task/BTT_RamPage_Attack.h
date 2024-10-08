// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Creature/Monster/RamPage/E_RamPage_Attacks.h"
#include "BTT_RamPage_Attack.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UBTT_RamPage_Attack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTT_RamPage_Attack();


protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


protected:
	void FinishTask();


protected:
	UPROPERTY(EditAnywhere)
	ERamPage_Attack AttackName = ERamPage_Attack::Defualt;

	TObjectPtr<UBehaviorTreeComponent> BTComponentOwner;

	FDelegateHandle EndTaskHandle;
	FDelegateHandle InterruptedTaskHandle;
};
