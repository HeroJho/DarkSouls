// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DK_AOE_Base.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnAOEOverlapActorDelegate, AActor*);

UCLASS()
class DARKSOULS_API ADK_AOE_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADK_AOE_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	void InitOption(float InRadius, bool InbDrawDebugSphere = true, bool InbIgnoreInstigator = true, bool InbTriggerOnBeginPlay = false);
	void Trigger();


protected:
	TObjectPtr<USceneComponent> SceneRootComponent;

	float Radius;
	bool bIgnoreInstigator;
	bool bDrawDebugSphere;
	bool bTriggerOnBeginPlay;


	// Delegate
public:
	FOnAOEOverlapActorDelegate OnAOEOverlapActorDelegate;
};
