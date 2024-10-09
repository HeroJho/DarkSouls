// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_RamPage_Attack.h"
#include "AIController.h"

#include "Creature/Monster/RamPage/DK_RamPage.h"
#include "Component/Combo/DK_ComboComponent.h"



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



	switch (AttackName)
	{
	case ERamPage_Attack::Defualt:
	{
		return EBTNodeResult::Failed;
	}
		break;
	case ERamPage_Attack::Combo1:
	{
		if (!RPOwner->Skill_Combo0())
		{
			return EBTNodeResult::Failed;
		}
	}
		break;
	case ERamPage_Attack::Combo2:
	{
		if (!RPOwner->Skill_Combo1())
		{
			return EBTNodeResult::Failed;
		}
	}
	break;
	case ERamPage_Attack::Combo3:
	{
		if (!RPOwner->Skill_Combo2())
		{
			return EBTNodeResult::Failed;
		}
	}
	break;
	case ERamPage_Attack::GroundSmash:
	{
		if (!RPOwner->GroundSmash())
		{
			return EBTNodeResult::Failed;
		}
	}
		break;
	case ERamPage_Attack::JumpAttack:
	{
		if (!RPOwner->JumpAttack())
		{
			return EBTNodeResult::Failed;
		}
	}
		break;
	case ERamPage_Attack::ThrowWall:
	{
		if (!RPOwner->ThrowWall())
		{
			return EBTNodeResult::Failed;
		}
	}
	break;
	default:
		break;
	}


	RPOwner->GetComboComponent()->OnComboInterruptedDelegate.Remove(InterruptedTaskHandle);
	RPOwner->GetComboComponent()->OnComboEndDelegate.Remove(EndTaskHandle);
	InterruptedTaskHandle = RPOwner->GetComboComponent()->OnComboInterruptedDelegate.AddUObject(this, &UBTT_RamPage_Attack::FinishTask);
	EndTaskHandle = RPOwner->GetComboComponent()->OnComboEndDelegate.AddUObject(this, &UBTT_RamPage_Attack::FinishTask);

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


