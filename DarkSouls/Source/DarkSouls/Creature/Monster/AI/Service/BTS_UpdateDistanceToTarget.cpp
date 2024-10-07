// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Service/BTS_UpdateDistanceToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTS_UpdateDistanceToTarget::UBTS_UpdateDistanceToTarget()
{
}




void UBTS_UpdateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();

	AActor* Target = Cast<AActor>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (IsValid(Target) == false)
		return;
	
		
	float Distance = Target->GetDistanceTo(Owner);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, Distance);
}
