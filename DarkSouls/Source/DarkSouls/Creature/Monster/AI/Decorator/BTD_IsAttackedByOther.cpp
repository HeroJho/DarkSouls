// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Decorator/BTD_IsAttackedByOther.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"





UBTD_IsAttackedByOther::UBTD_IsAttackedByOther()
{
	NodeName = TEXT("IsAttackedByOther");

}

bool UBTD_IsAttackedByOther::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);


	bool bIsAttacked = OwnerComp.GetBlackboardComponent()->GetValueAsBool(BlackboardKey.SelectedKeyName);

	return bIsAttacked;
}
