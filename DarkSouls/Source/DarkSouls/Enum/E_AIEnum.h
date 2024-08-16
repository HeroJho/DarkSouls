// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "E_AIEnum.generated.h"



UENUM(BlueprintType)
enum class EAIState : uint8
{
	Passive UMETA(DisplayName = "Passive"),
	Attacking UMETA(DisplayName = "Attacking"),
	Frozen UMETA(DisplayName = "Frozen"),
	Investigating UMETA(DisplayName = "Investigating"),
	Seeking UMETA(DisplayName = "Seeking"),
	Dead UMETA(DisplayName = "Dead"),
};


UENUM(BlueprintType)
enum class EAISense : uint8
{
	None UMETA(DisplayName = "None"),
	Sight UMETA(DisplayName = "Sight"),
	Hearing UMETA(DisplayName = "Hearing"),
	Damage UMETA(DisplayName = "Damage"),
};


UCLASS()
class DARKSOULS_API UE_AIEnum : public UObject
{
	GENERATED_BODY()
	
};
