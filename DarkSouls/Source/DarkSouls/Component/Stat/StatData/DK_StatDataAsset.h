// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DK_StatDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_StatDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDK_StatDataAsset();


public:
	UPROPERTY(EditAnywhere, Category = Stat)
	int32 MaxHP;


};
