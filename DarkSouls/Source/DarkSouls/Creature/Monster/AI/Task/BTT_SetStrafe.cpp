// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_SetStrafe.h"
#include "AIController.h"

#include "Creature/DK_Creature.h"

EBTNodeResult::Type UBTT_SetStrafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	ADK_Creature* Creature = Cast<ADK_Creature>(Owner);
	if (IsValid(Creature) == false)
	{
		return EBTNodeResult::Failed;
	}

	Creature->SetStrafe(bIsStrafe);


	return EBTNodeResult::Succeeded;
}
