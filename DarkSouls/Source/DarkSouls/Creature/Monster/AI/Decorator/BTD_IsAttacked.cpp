// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Decorator/BTD_IsAttacked.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Creature/DK_Object.h"


UBTD_IsAttacked::UBTD_IsAttacked()
{
	bNotifyBecomeRelevant = true;
	bNotifyCeaseRelevant = true;

	//bNotifyDeactivation = true;
	//bNotifyActivation = true;
}

bool UBTD_IsAttacked::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ADK_Object* Owner = Cast<ADK_Object>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(Owner) == false)
		return false;

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("AAAAAAAAAAAAAAA")));

	return Owner->GetSmallHittedTrigger();
}

