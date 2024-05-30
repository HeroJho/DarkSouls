// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DK_ComboActionData.generated.h"



USTRUCT()
struct FAttackColInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FString> AttackCollisions;

};

UCLASS()
class DARKSOULS_API UDK_ComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UDK_ComboActionData();


public:
	UPROPERTY(EditAnywhere, Category = Combo)
	TObjectPtr<class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, Category = Combo)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Combo)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Collision)
	TArray<FAttackColInfo> AttackColInfos;
	

};
