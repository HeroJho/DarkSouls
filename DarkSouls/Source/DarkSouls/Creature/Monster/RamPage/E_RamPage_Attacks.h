// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "E_RamPage_Attacks.generated.h"



UENUM(BlueprintType)
enum class ERamPage_Attack : uint8
{
	Defualt UMETA(DisplayName = "Defualt"),
	Combo1 UMETA(DisplayName = "Combo1"),
	GroundSmash UMETA(DisplayName = "GroundSmash"),
	JumpAttack UMETA(DisplayName = "JumpAttack"),
};





/**
 * 
 */
UCLASS()
class DARKSOULS_API UE_RamPage_Attacks : public UObject
{
	GENERATED_BODY()
	
};
