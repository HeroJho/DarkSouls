// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DK_ComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_ComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDK_ComboActionData();


public:
	UPROPERTY(EditAnywhere, Category = Name)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

};
