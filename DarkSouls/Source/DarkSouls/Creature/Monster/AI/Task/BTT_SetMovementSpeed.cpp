// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_SetMovementSpeed.h"
#include "AIController.h"

#include "Creature/DK_Creature.h"


UBTT_SetMovementSpeed::UBTT_SetMovementSpeed()
{
}

EBTNodeResult::Type UBTT_SetMovementSpeed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	ADK_Creature* Creature = Cast<ADK_Creature>(Owner);
	if (IsValid(Creature) == false)
	{
		return EBTNodeResult::Failed;
	}

	Creature->SetMovementSpeed(SpeedMode);

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTT_SetMovementSpeed::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);




	return EBTNodeResult::Type();
}

