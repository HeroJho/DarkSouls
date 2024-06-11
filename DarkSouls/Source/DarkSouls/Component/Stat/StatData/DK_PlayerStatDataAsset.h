// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/StatData/DK_StatDataAsset.h"
#include "DK_PlayerStatDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_PlayerStatDataAsset : public UDK_StatDataAsset
{
	GENERATED_BODY()

public:
	UDK_PlayerStatDataAsset();

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 MaxSP;
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 MaxTP;

};
