// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_RamPage_Attack.h"
#include "AIController.h"

#include "Creature/Monster/RamPage/DK_RamPage.h"
#include "Component/Combo/DK_ComboComponent.h"



UBTT_RamPage_Attack::UBTT_RamPage_Attack()
{

	bNotifyTick = false;
	bNotifyTaskFinished = true;

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
	case ERamPage_Attack::FullWall:
	{
		OnAbortTaskDelegate.Clear();
		OnAbortTaskDelegate.AddUObject(RPOwner, &ADK_RamPage::DestroyWall);

		if (!RPOwner->FullWall())
		{
			return EBTNodeResult::Failed;
		}
	}
		break;	
	case ERamPage_Attack::ThrowWall:
	{
		OnAbortTaskDelegate.Clear();
		OnAbortTaskDelegate.AddUObject(RPOwner, &ADK_RamPage::DestroyWall);

		if (!RPOwner->ThrowWall(false))
		{
			return EBTNodeResult::Failed;
		}
	}
		break;
	case ERamPage_Attack::SpeedThrowWall:
	{
		OnAbortTaskDelegate.Clear();
		OnAbortTaskDelegate.AddUObject(RPOwner, &ADK_RamPage::DestroyWall);

		if (!RPOwner->ThrowWall(true))
		{
			return EBTNodeResult::Failed;
		}
	}
	break;
	case ERamPage_Attack::BackJump:
	{
		if (!RPOwner->BackJump())
		{
			return EBTNodeResult::Failed;
		}
	}
		break;
	default:
		break;
	}
	

	RPOwner->GetComboComponent()->OnComboEndForTaskDelegate.Clear();
	//RPOwner->GetComboComponent()->OnComboInterruptedForTaskDelegate.Clear();

	//RPOwner->GetComboComponent()->OnComboInterruptedForTaskDelegate.AddUObject(this, &UBTT_RamPage_Attack::FinishTask);
	RPOwner->GetComboComponent()->OnComboEndForTaskDelegate.AddUObject(this, &UBTT_RamPage_Attack::FinishTask);

	return EBTNodeResult::InProgress;
}


void UBTT_RamPage_Attack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);


	switch (TaskResult)
	{
	case EBTNodeResult::Succeeded:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Succeeded")));
	}
		break;
	case EBTNodeResult::Failed:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Failed")));
		OnAbortTaskDelegate.Broadcast();
	}
		break;
	case EBTNodeResult::Aborted:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Aborted")));
		OnAbortTaskDelegate.Broadcast();
	}
		break;
	case EBTNodeResult::InProgress:
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("InProgress")));
	}
		break;
	default:
		break;
	}


}




void UBTT_RamPage_Attack::FinishTask()
{
	 FinishLatentTask(*BTComponentOwner, EBTNodeResult::Succeeded);
}


