// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Decorator/BTD_IsWithinIdealRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


bool UBTD_IsWithinIdealRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	if (IsValid(Owner) == false)
		return false;

	AActor* Target = Cast<AActor>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (IsValid(Target) == false)
		return false;



	float Distance = Target->GetDistanceTo(Owner) - ErrorMargin;
	if (Distance <= ErrorMargin)
	{
		return true;
	}


	return false;
}
