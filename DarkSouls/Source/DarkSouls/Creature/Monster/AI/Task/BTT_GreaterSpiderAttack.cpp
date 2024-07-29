// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_GreaterSpiderAttack.h"
#include "AIController.h"

#include "Creature/Monster/DK_Greater_Spider.h"


UBTT_GreaterSpiderAttack::UBTT_GreaterSpiderAttack()
{

	bNotifyTick = false;


}

EBTNodeResult::Type UBTT_GreaterSpiderAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	BTComponentOwner = OwnerComp;

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	ADK_Greater_Spider* SpiderOwner = Cast<ADK_Greater_Spider>(Owner);
	if (IsValid(SpiderOwner) == false)
	{
		return EBTNodeResult::Failed;
	}

	////	 *인자있는 함수 Delay
	//FTimerDelegate TimerDelegate;
	//TimerDelegate.BindUFunction(this, FName("FinishTask"), &OwnerComp, EBTNodeResult::Succeeded);
	//FTimerHandle UnusedHandle;
	//SpiderOwner->GetWorldTimerManager().SetTimer(
	//	UnusedHandle, TimerDelegate, 20.f, false);

	SpiderOwner->Skill_Combo0();
	SpiderOwner->OnAttackEnd.AddUObject(this, &UBTT_GreaterSpiderAttack::FinishTask);



	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_GreaterSpiderAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTT_GreaterSpiderAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}

void UBTT_GreaterSpiderAttack::FinishTask()
{
	FinishLatentTask(*BTComponentOwner, EBTNodeResult::Succeeded);
}
