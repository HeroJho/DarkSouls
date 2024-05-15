// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DK_OptionManager.h"

// Sets default values for this component's properties
UDK_OptionManager::UDK_OptionManager()
	:MouseSensitivity(1.f)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_OptionManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

