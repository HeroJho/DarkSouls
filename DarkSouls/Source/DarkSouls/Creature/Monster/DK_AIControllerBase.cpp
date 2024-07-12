// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/DK_AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Damage.h"

#include "Creature/DK_Creature.h"
#include "Interface/DK_DamageableInterface.h"


const FName ADK_AIControllerBase::AIStateKey(TEXT("AIState"));
const FName ADK_AIControllerBase::AttackTargetKey(TEXT("AttackTarget"));


ADK_AIControllerBase::ADK_AIControllerBase()
{

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

}


void ADK_AIControllerBase::BeginPlay()
{
	Super::BeginPlay();

}

void ADK_AIControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunAI(InPawn);

	FScriptDelegate PerceptionUpdatedDelegate;
	PerceptionUpdatedDelegate.BindUFunction(this, FName("OnPerceptionUpdated_Notify"));
	AIPerceptionComponent->OnPerceptionUpdated.AddUnique(PerceptionUpdatedDelegate);

	SetStateAsPassive();
}

void ADK_AIControllerBase::RunAI(APawn* InPawn)
{
	ADK_Creature* CreaturePawn = Cast<ADK_Creature>(InPawn);
	if (!IsValid(CreaturePawn))
		return;

	UBehaviorTree* BTAsset = CreaturePawn->GetBTAsset();
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

	// ������ Ÿ������ ������ �ų�?
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

	// �׾���?
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
}

void ADK_AIControllerBase::SetStateAsSeeking(FVector Location)
{
}

void ADK_AIControllerBase::SetStateAsFrozen()
{
}

void ADK_AIControllerBase::SetStateAsDead()
{
}








void ADK_AIControllerBase::OnPerceptionUpdated_Notify(const TArray<AActor*>& UpdatedActors)
{
	// ������Ʈ�� �νĵưų�, ����� �� ȣ��ȴ�.

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString(TEXT("sdfsfdsfsfdfs")));

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
		// TODO: Don't seek target if seen again
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

}

void ADK_AIControllerBase::HandleSensedSound(FVector Location)
{
}

void ADK_AIControllerBase::HandleSensedDamage(AActor* Actor)
{
}
