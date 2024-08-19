// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Task/BTT_MoveToIdealRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Tasks/AITask_MoveTo.h"




EBTNodeResult::Type UBTT_MoveToIdealRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerCompTree = &OwnerComp;

	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Owner = Controller->GetPawn();
	if (IsValid(Owner) == false)
		return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(
		OwnerComp.GetBlackboardComponent()->GetValueAsObject(AttackTargetKey.SelectedKeyName));
	if (IsValid(Target) == false)
		return EBTNodeResult::Failed;


	//UAITask_MoveTo* MoveToTask = UAITask_MoveTo::AIMoveTo(Controller, FVector(), Target, IdealRange,);
	//FScriptDelegate FinishedDelegate;
	//FinishedDelegate.BindUFunction(this, FName("OnMoveCompleted"));
	//MoveToTask->OnMoveFinished.AddUnique(FinishedDelegate);
	//FScriptDelegate FailedDelegate;
	//FailedDelegate.BindUFunction(this, FName("OnMoveFailed"));
	//MoveToTask->OnRequestFailed.AddUnique(FailedDelegate);

	//MoveToTask->ReadyForActivation();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTT_MoveToIdealRange::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	Controller->StopMovement();

	return EBTNodeResult::Failed;
}


//void UBTT_MoveToIdealRange::OnMoveFailed()
//{
//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString("OnMoveFailed"));
//
//	FinishLatentTask(*OwnerCompTree, EBTNodeResult::Failed);
//}
//
//void UBTT_MoveToIdealRange::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
//{
//	if (Result.IsSuccess())
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString("Success"));
//		FinishLatentTask(*OwnerCompTree, EBTNodeResult::Succeeded);
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString("Failed"));
//		FinishLatentTask(*OwnerCompTree, EBTNodeResult::Failed);
//	}
//
//}

