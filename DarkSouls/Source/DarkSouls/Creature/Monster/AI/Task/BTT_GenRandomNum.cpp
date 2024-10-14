// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_GenRandomNum.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTT_GenRandomNum::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	float GendNum = FMath::RandRange(Min, Max);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(GenedNumber.SelectedKeyName, GendNum);

	return EBTNodeResult::Succeeded;


}
