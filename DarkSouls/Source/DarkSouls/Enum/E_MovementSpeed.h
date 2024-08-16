// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "E_MovementSpeed.generated.h"



UENUM(BlueprintType)
enum class EMovementSpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walking UMETA(DisplayName = "Walking"),
	Jogging UMETA(DisplayName = "Jogging"),
	Sprinting UMETA(DisplayName = "Sprinting"),
};



/**
 * 
 */
UCLASS()
class DARKSOULS_API UE_MovementSpeed : public UObject
{
	GENERATED_BODY()
	
};
