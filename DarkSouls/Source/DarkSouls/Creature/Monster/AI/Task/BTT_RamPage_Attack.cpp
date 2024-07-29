// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_RamPage_Attack.h"
#include "AIController.h"

#include "Creature/Monster/RamPage/DK_RamPage.h"



UBTT_RamPage_Attack::UBTT_RamPage_Attack()
{

	bNotifyTick = false;


}

EBTNodeResult::Type UBTT_RamPage_Attack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	BTComponentOwner = OwnerComp;

	APawn* Owner = OwnerComp.GetAIOwner()->GetPawn();
	ADK_RamPage* RPOwner = Cast<ADK_RamPage>(Owner);
	if (IsValid(RPOwner) == false)
	{
		return EBTNodeResult::Failed;
	}

	RPOwner->Skill_Combo0();
	RPOwner->OnAttackEnd.AddUObject(this, &UBTT_RamPage_Attack::FinishTask);



	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_RamPage_Attack::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTT_RamPage_Attack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
}

void UBTT_RamPage_Attack::FinishTask()
{
	FinishLatentTask(*BTComponentOwner, EBTNodeResult::Succeeded);
}
