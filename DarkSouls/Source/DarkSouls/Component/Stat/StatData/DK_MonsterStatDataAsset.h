// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Component/Stat/StatData/DK_StatDataAsset.h"
#include "DK_MonsterStatDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_MonsterStatDataAsset : public UDK_StatDataAsset
{
	GENERATED_BODY()

public:
	UDK_MonsterStatDataAsset();

public:
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 MaxGP;


};
