// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_SetFocus.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Creature/Monster/DK_AIControllerBase.h"


EBTNodeResult::Type UBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ADK_AIControllerBase* Controller = Cast<ADK_AIControllerBase>(OwnerComp.GetAIOwner());


	if (IsValid(Controller) == false)
	{
		return EBTNodeResult::Failed;
	}

	if (FocusLocation.IsSet())
	{
		FVector Location = OwnerComp.GetBlackboardComponent()->GetValueAsVector(FocusLocation.SelectedKeyName);
		Controller->SetFocalPoint(Location);

		return EBTNodeResult::Succeeded;
	}
		
	if (bIsFocus)
		Controller->SetFocusTarget(360.f);
	else
		Controller->ClearFocusTarget();
	

	return EBTNodeResult::Succeeded;
}
