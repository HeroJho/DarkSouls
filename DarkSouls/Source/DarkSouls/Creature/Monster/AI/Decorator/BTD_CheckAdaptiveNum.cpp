// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Decorator/BTD_CheckAdaptiveNum.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Component/AdaptiveGenNum/DK_AdaptiveGenNumComponent.h"


bool UBTD_CheckAdaptiveNum::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);


	int GendNum = OwnerComp.GetBlackboardComponent()->GetValueAsInt(GenedNumKey.SelectedKeyName);
	if (GendNum != Num)
		return false;


	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(Owner) == false)
		return false;

	UDK_AdaptiveGenNumComponent* AdaptiveComp = Owner->GetComponentByClass<UDK_AdaptiveGenNumComponent>();
	if (AdaptiveComp == nullptr)
		return false;

	AdaptiveComp->CalculAdaptiveToken(TableIndex, GendNum);

	return true;
}
