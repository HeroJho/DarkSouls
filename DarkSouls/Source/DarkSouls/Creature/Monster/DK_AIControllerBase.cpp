// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/DK_AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Creature/DK_Creature.h"
#include "Interface/DK_DamageableInterface.h"


const FName ADK_AIControllerBase::AIStateKey(TEXT("AIState"));
const FName ADK_AIControllerBase::AttackTargetKey(TEXT("AttackTarget"));
const FName ADK_AIControllerBase::LocationOfInterestKey(TEXT("LocationOfInterest"));
const FName ADK_AIControllerBase::bIsAttackedKey(TEXT("bIsAttacked"));


ADK_AIControllerBase::ADK_AIControllerBase()
{

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	
	TimetoSeekAfterLosingSight = 2.f;

}


void ADK_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();


}



void ADK_AIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	CreatureOwner = Cast<ADK_Creature>(InPawn);

	RunAI(InPawn);

	FScriptDelegate PerceptionUpdatedDelegate;
	PerceptionUpdatedDelegate.BindUFunction(this, FName("OnPerceptionUpdated_Notify"));
	AIPerceptionComponent->OnPerceptionUpdated.AddUnique(PerceptionUpdatedDelegate);
	FScriptDelegate PerceptionPorgottenDelegate;
	PerceptionPorgottenDelegate.BindUFunction(this, FName("OnPerceptionTargetForgotten_Notify"));
	AIPerceptionComponent->OnTargetPerceptionForgotten.AddUnique(PerceptionPorgottenDelegate);

	SetStateAsPassive();
}





void ADK_AIControllerBase::SetFocusTarget(float RotationSpeed)
{
	AActor* Target = GetAttackTarget();
	if (!IsValid(Target))
	{
		return;
	}

	UCharacterMovementComponent* Movement = CreatureOwner->GetCharacterMovement();
	Movement->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
	Movement->bOrientRotationToMovement = false;
	SetFocus(Target);
}

void ADK_AIControllerBase::ClearFocusTarget()
{
	UCharacterMovementComponent* Movement = CreatureOwner->GetCharacterMovement();
	Movement->RotationRate = FRotator(0.f, 360.f, 0.f);
	Movement->bOrientRotationToMovement = true;
	ClearFocus(EAIFocusPriority::Gameplay);
}

AActor* ADK_AIControllerBase::GetAttackTarget()
{
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey));
	return Target;
}

void ADK_AIControllerBase::RunAI(APawn* InPawn)
{
	UBehaviorTree* BTAsset = CreatureOwner->GetBTAsset();
	if (BTAsset == nullptr)
		return;

	RunBehaviorTree(BTAsset);

}

void ADK_AIControllerBase::StopAI()
{
	UBehaviorTreeComponent* BehaviorTreeComponent = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BehaviorTreeComponent) return;

	BehaviorTreeComponent->StopTree(EBTStopMode::Safe);

}






EAIState ADK_AIControllerBase::GetCurrentState()
{
	return static_cast<EAIState>(Blackboard->GetValueAsEnum(AIStateKey));
}

void ADK_AIControllerBase::SetStateAsPassive()
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int8>(EAIState::Passive));
}

void ADK_AIControllerBase::SetStateAsAttacking(AActor* AttackTarget, bool bUseLastKnownAttack)
{
	UObject* TempAttackTarget = AttackTarget;

	// 마지막 타겟으로 설정할 거냐?
	if (bUseLastKnownAttack)
	{
		UObject* Target = Blackboard->GetValueAsObject(AttackTargetKey);
		if (IsValid(Target))
			TempAttackTarget = Target;
	}

	if (!IsValid(TempAttackTarget))
	{
		SetStateAsPassive();
		return;
	}

	// 죽었냐?
	IDK_DamageableInterface* DamageableTarget = Cast<IDK_DamageableInterface>(TempAttackTarget);
	if (DamageableTarget == nullptr || DamageableTarget->IsDead())
	{
		SetStateAsPassive();
		return;
	}

	Blackboard->SetValueAsObject(AttackTargetKey, TempAttackTarget);
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int8>(EAIState::Attacking));

}

void ADK_AIControllerBase::SetStateAsInvestigating(FVector Location)
{
	Blackboard->SetValueAsVector(LocationOfInterestKey, Location);
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int32>(EAIState::Investigating));
}

void ADK_AIControllerBase::SetStateAsSeeking(FVector Location)
{
	Blackboard->SetValueAsVector(LocationOfInterestKey, Location);
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int8>(EAIState::Seeking));
}

void ADK_AIControllerBase::SetStateAsFrozen()
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int32>(EAIState::Frozen));
}

void ADK_AIControllerBase::SetStateAsDead()
{
	Blackboard->SetValueAsEnum(AIStateKey, static_cast<int32>(EAIState::Dead));
}








void ADK_AIControllerBase::OnPerceptionUpdated_Notify(const TArray<AActor*>& UpdatedActors)
{
	// 오브젝트가 인식됐거나, 벗어났을 때 호출된다.

	for (int32 i = 0; i < UpdatedActors.Num(); ++i)
	{
		AActor* CurActor = UpdatedActors[i];

		FAIStimulus Stimulus;

		if (CanSenseActor(Stimulus, CurActor, EAISense::Sight))
		{
			HandleSensedSight(CurActor);
		}
		else
		{
			HandleLostSight(CurActor);
		}


		if (CanSenseActor(Stimulus, CurActor, EAISense::Hearing))
		{
			HandleSensedSound(Stimulus.StimulusLocation);
		}


		if (CanSenseActor(Stimulus, CurActor, EAISense::Damage))
		{
			HandleSensedDamage(CurActor);
		}


	}


}

void ADK_AIControllerBase::OnPerceptionTargetForgotten_Notify(AActor* Actor)
{
	HandleForgetTarget(Actor);
}




bool ADK_AIControllerBase::CanSenseActor(FAIStimulus& OUT_Stimulus, AActor* Actor, EAISense Sense)
{
	FActorPerceptionBlueprintInfo PerceptionInfo;
	AIPerceptionComponent->GetActorsPerception(Actor, PerceptionInfo);

	for (int32 i = 0; i < PerceptionInfo.LastSensedStimuli.Num(); ++i)
	{
		TSubclassOf<UAISense> SenseClass = UAIPerceptionSystem::GetSenseClassForStimulus(GetWorld(), PerceptionInfo.LastSensedStimuli[i]);
		
		switch (Sense)
		{
			case EAISense::None:
				break;
			case EAISense::Sight:
			{
				if (SenseClass == UAISense_Sight::StaticClass())
				{
					OUT_Stimulus = PerceptionInfo.LastSensedStimuli[i];
					return PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed();
				}
			}
				break;
			case EAISense::Hearing:
			{
				if (SenseClass == UAISense_Hearing::StaticClass())
				{
					OUT_Stimulus = PerceptionInfo.LastSensedStimuli[i];
					return PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed();
				}
			}
				break;
			case EAISense::Damage:
			{
				if (SenseClass == UAISense_Damage::StaticClass())
				{
					OUT_Stimulus = PerceptionInfo.LastSensedStimuli[i];
					return PerceptionInfo.LastSensedStimuli[i].WasSuccessfullySensed();
				}
			}
				break;

		}

	}


	return false;
}



void ADK_AIControllerBase::HandleSensedSight(AActor* Actor)
{
	switch (GetCurrentState())
	{
	case EAIState::Passive:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Attacking:
	{
		// Don't seek target if seen again
		// 시야에서 벗어낫다 들어왔다를 빠르게 반복하는 경우
		UObject* CurTarget = Blackboard->GetValueAsObject(AttackTargetKey);
		if (CurTarget == Actor)
		{
			GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);
		}
	}
		break;
	case EAIState::Frozen:
		break;
	case EAIState::Investigating:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Seeking:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Dead:
		break;
	}

}

void ADK_AIControllerBase::HandleLostSight(AActor* Actor)
{
	UObject* CurTarget = Blackboard->GetValueAsObject(AttackTargetKey);
	if (CurTarget != Actor)
	{
		return;
	}

	switch (GetCurrentState())
	{
	case EAIState::Passive:
		break;
	case EAIState::Attacking:
		SetTimerLosingSight();
		break;
	case EAIState::Frozen:
		break;
	case EAIState::Investigating:
		SetTimerLosingSight();
		break;
	case EAIState::Seeking:
		break;
	case EAIState::Dead:
		break;
	}

}

void ADK_AIControllerBase::HandleSensedSound(FVector Location)
{
	switch (GetCurrentState())
	{
	case EAIState::Passive:
		SetStateAsInvestigating(Location);
		break;
	case EAIState::Attacking:
		break;
	case EAIState::Frozen:
		break;
	case EAIState::Investigating:
		SetStateAsInvestigating(Location);
		break;
	case EAIState::Seeking:
		SetStateAsInvestigating(Location);
		break;
	case EAIState::Dead:
		break;
	default:
		break;
	}
}

void ADK_AIControllerBase::HandleSensedDamage(AActor* Actor)
{
	switch (GetCurrentState())
	{
	case EAIState::Passive:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Attacking:
		break;
	case EAIState::Frozen:
		break;
	case EAIState::Investigating:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Seeking:
		SetStateAsAttacking(Actor, false);
		break;
	case EAIState::Dead:
		break;
	default:
		break;
	}
}

void ADK_AIControllerBase::HandleForgetTarget(AActor* Actor)
{
	UObject* CurTarget = Blackboard->GetValueAsObject(AttackTargetKey);
	if (CurTarget == Actor)
	{
		SetStateAsPassive();
	}

}

void ADK_AIControllerBase::SetTimerLosingSight()
{
	GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);
	GetWorldTimerManager().SetTimer(SeekAttackTargetTimerHandle, this, &ADK_AIControllerBase::SeekTarget, TimetoSeekAfterLosingSight, false);

}

void ADK_AIControllerBase::SeekTarget()
{
	GetWorldTimerManager().ClearTimer(SeekAttackTargetTimerHandle);

	AActor* CurTarget = Cast<AActor>(Blackboard->GetValueAsObject(AttackTargetKey));
	if (!IsValid(CurTarget))
	{
		SetStateAsPassive();
		return;
	}

	SetStateAsSeeking(CurTarget->GetActorLocation());
}






void ADK_AIControllerBase::SetbIsAttackedKey(bool bValue)
{
	Blackboard->SetValueAsBool(bIsAttackedKey, bValue);
}
