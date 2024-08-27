// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Attack/DK_AttackComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"

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




}

bool UDK_AttackComponent::JumpToAttackTarget(AActor* Target)
{
	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());

	AActor* Owner = GetOwner();

	FVector OwnerLocation = Owner->GetTargetLocation();
	FVector FutureLocation = GameMode->GetToolManager()->CalculateFutureActorXYLocation(Target, 1.f);
	FutureLocation.Z = 100.f;

	
	float Distance = Owner->GetDistanceTo(Target);
	float Rad = UKismetMathLibrary::FClamp(UKismetMathLibrary::NormalizeToRange(Distance, 400.f, 800.f), 0.f, 1.f);
	float Arc = UKismetMathLibrary::Lerp(0.5f, 0.94f, Rad);


	FVector OutLaunchVelocity;

	bool bResult = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		this,
		OutLaunchVelocity,
		OwnerLocation,
		FutureLocation,
		0.f,
		0.5f
	);

	DrawDebugCapsule(GetWorld(), FutureLocation, 100.f, 100.f, FQuat::Identity, FColor::Cyan, false, 1.f);

	if (bResult == false)
		return false;

	ACharacter* CharacterOwner = Cast<ACharacter>(Owner);
	if (IsValid(CharacterOwner) == false)
		return false;

	CharacterOwner->LaunchCharacter(OutLaunchVelocity, true, true);

	/*FScriptDelegate LandDelegate;
	LandDelegate.BindUFunction();
	CharacterOwner->LandedDelegate.AddUnique(,);*/

	// ÀÜµð Å×½ºÆ®

	return true;
}

