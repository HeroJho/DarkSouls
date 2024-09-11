// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Attack/DK_AttackComponent.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"

#include "Struct/S_DamageInfo.h"
#include "Interface/DK_DamageableInterface.h"
#include "AOE/DK_AOE_Base.h"
#include "Game/DK_GameMode.h"
#include "Manager/DK_ToolManager.h"



// Sets default values for this component's properties
UDK_AttackComponent::UDK_AttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_AttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
		
}

void UDK_AttackComponent::AOEDamage(FVector SpawnLocation, float Radius, FS_DamageInfo DamageInfo, bool bIsRenderDebug)
{
	const FTransform SpawnTransform(SpawnLocation);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	ADK_AOE_Base* SlashAOE = GetWorld()->SpawnActorDeferred<ADK_AOE_Base>(ADK_AOE_Base::StaticClass(), SpawnTransform, OwnerPawn, OwnerPawn);
	SlashAOE->InitOption(300.f, bIsRenderDebug, true, true);

	SlashAOE->OnAOEOverlapActorDelegate.AddLambda([DamageInfo, OwnerPawn](AActor* HitActor)
		{
			if (!IsValid(OwnerPawn))
				return;

			IDK_DamageableInterface* HitActorDamageable = Cast<IDK_DamageableInterface>(HitActor);
			if (HitActorDamageable)
			{
				HitActorDamageable->TakeDamage(DamageInfo, OwnerPawn);
			}
		});

	SlashAOE->FinishSpawning(SpawnTransform);


	NewObject<ADK_AOE_Base>();

}

bool UDK_AttackComponent::JumpToAttackTarget(AActor* Target, UCurveFloat* Curve)
{
	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	JumpDeltaTimeAcc = 0.f;
	bIsPuase = true;

	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());

	TArray<FVector> Poss;
	GameMode->GetToolManager()->PredictProjectilePath(GetOwner(), Target, Poss, true);

	double Dis = FVector::Distance(Target->GetTargetLocation(), Poss.Last());

	ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
	// CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);
	float EndTime = CharacterOwner->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(1);

	FTimerDelegate JumpTimerDelegate;
	JumpTimerDelegate.BindUFunction(this, FName("JumpTick"), Curve, Poss, EndTime);
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 0.01f, true);


	return true;
}


void UDK_AttackComponent::JumpTick(UCurveFloat* Curve, TArray<FVector> Poss, float EndAnimLength)
{
	const float TimeDelta = GetWorld()->DeltaTimeSeconds;
	const float Speed = 0.5f;

	JumpDeltaTimeAcc = FMath::Clamp(JumpDeltaTimeAcc + TimeDelta * Speed, 0.f, 1.f);
	float CurveValue = Curve->GetFloatValue(JumpDeltaTimeAcc);

	int CurIndex = CurveValue * Poss.Num();
	int NexIndex = CurIndex + 1;

	float TotalTime = 1.f / Speed;
	float EndRatio = EndAnimLength / TotalTime;



	if (bIsPuase && 1.f - EndRatio <= JumpDeltaTimeAcc)
	{
		ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
		CharacterOwner->GetMesh()->GetAnimInstance()->Montage_Resume(nullptr);
		bIsPuase = false;
	}

	// ³¡
	if (NexIndex >= Poss.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
		// GetOwner()->SetActorLocation(Poss[CurIndex]);

		return;
	}

	// 0~0.1    0~1  
	float LerpAcc = (CurveValue * Poss.Num()) - CurIndex;
	FVector DestPos = UKismetMathLibrary::VLerp(Poss[CurIndex], Poss[NexIndex], LerpAcc);
	
	GetOwner()->SetActorLocation(DestPos, true);
	
}

