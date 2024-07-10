// Fill out your copyright notice in the Description page of Project Settings.


#include "AOE/DK_AOE_Base.h"

#include "Kismet/KismetSystemLibrary.h"



// Sets default values
ADK_AOE_Base::ADK_AOE_Base()
{
 	PrimaryActorTick.bCanEverTick = false;

	Radius = 100.f;
	bIgnoreInstigator = true;
	bDrawDebugSphere = true;
	bTriggerOnBeginPlay = false;

}

void ADK_AOE_Base::InitOption(float InRadius, bool InbDrawDebugSphere, bool InbIgnoreInstigator, bool InbTriggerOnBeginPlay)
{
	Radius = InRadius;
	bDrawDebugSphere = InbDrawDebugSphere;
	bIgnoreInstigator = InbIgnoreInstigator;
	bTriggerOnBeginPlay = InbTriggerOnBeginPlay;

}

// Called when the game starts or when spawned
void ADK_AOE_Base::BeginPlay()
{
	Super::BeginPlay();
		
	if (bTriggerOnBeginPlay)
	{
		FTimerHandle TempTimerHandle;
		GetWorldTimerManager().SetTimer(TempTimerHandle, this, &ADK_AOE_Base::Trigger, 0.05f, false);
	}

}

void ADK_AOE_Base::Trigger()
{
	if (bDrawDebugSphere)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 12, FColor::Magenta, false, 2.f, 0, 1.f);
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ColObjectTypes;
	ColObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> IgnoreActors;
	if (bIgnoreInstigator)
	{
		IgnoreActors.Add(GetInstigator());
	}

	TArray<AActor*> OutActors;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), Radius, ColObjectTypes, nullptr, IgnoreActors, OutActors);
	
	for (int32 i = 0; i < OutActors.Num(); ++i)
	{
		OnAOEOverlapActorDelegate.Broadcast(OutActors[i]);
	}

}

