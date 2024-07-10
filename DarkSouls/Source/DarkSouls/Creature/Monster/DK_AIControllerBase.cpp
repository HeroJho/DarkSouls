// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/DK_AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"

ADK_AIControllerBase::ADK_AIControllerBase()
{
}


void ADK_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void ADK_AIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI();

}

void ADK_AIControllerBase::RunAI()
{
	RunBehaviorTree(BTAsset);

}

void ADK_AIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent) return;

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);

}
