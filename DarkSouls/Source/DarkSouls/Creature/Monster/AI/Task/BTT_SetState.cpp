// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_SetState.h"
#include "AIController.h"

#include "Creature/DK_Creature.h"
#include "Creature/Monster/DK_AIControllerBase.h"


EBTNodeResult::Type UBTT_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);


	ADK_AIControllerBase* Controller = Cast<ADK_AIControllerBase>(OwnerComp.GetAIOwner());

	if (IsValid(Controller) == false)
	{
		return EBTNodeResult::Failed;
	}

	switch (State)
	{
	case EAIState::Defualt:
		break;
	case EAIState::Passive:
		Controller->SetStateAsPassive();
		break;
	case EAIState::Attacking:
		//Controller->SetStateAsAttacking();
		break;
	case EAIState::Frozen:
		//Controller->SetStateAsFrozen();
		break;
	case EAIState::Investigating:
		//Controller->SetStateAsInvestigating();
		break;
	case EAIState::Seeking:
		break;
	case EAIState::Dead:
		break;
	default:
		break;
	}


	return EBTNodeResult::Succeeded;
}
