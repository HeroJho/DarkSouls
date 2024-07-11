// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Struct/S_DamageInfo.h"
#include "DK_ComboActionData.generated.h"


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

	UPROPERTY(EditAnywhere, Category = Damage)
	TArray<FS_DamageInfo> DamageInfos;
	


};
