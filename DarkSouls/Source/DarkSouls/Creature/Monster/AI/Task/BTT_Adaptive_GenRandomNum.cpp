// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_Adaptive_GenRandomNum.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Component/AdaptiveGenNum/DK_AdaptiveGenNumComponent.h"




EBTNodeResult::Type UBTT_Adaptive_GenRandomNum::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (TableIndex == -1)
		return EBTNodeResult::Failed;

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(Owner) == false)
		return EBTNodeResult::Failed;


	UDK_AdaptiveGenNumComponent* AdaptiveComp = Owner->GetComponentByClass<UDK_AdaptiveGenNumComponent>();
	if(AdaptiveComp == nullptr)
		return EBTNodeResult::Failed;


	// 테이블이 없으면 만든다
	if(AdaptiveComp->IsExistTable(TableIndex) == false)
	{
		AdaptiveComp->CreateNewTable(TableIndex, NodeInfos);
	}

	int RandIndex = AdaptiveComp->GenAdaptiveNum(TableIndex);
	if(RandIndex == -1)
		return EBTNodeResult::Failed;


	OwnerComp.GetBlackboardComponent()->SetValueAsInt(GenedNumber.SelectedKeyName, RandIndex);


	return EBTNodeResult::Succeeded;
}
