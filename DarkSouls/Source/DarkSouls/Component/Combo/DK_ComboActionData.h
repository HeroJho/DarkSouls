// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DK_ComboActionData.generated.h"


USTRUCT()
struct FAttackInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Collision)
	TArray<FString> AttackCollisions;


	UPROPERTY(EditAnywhere, Category = Attack)
	float Damage;
	UPROPERTY(EditAnywhere, Category = Attack)
	uint8 bIsDown : 1;
	UPROPERTY(EditAnywhere, Category = Attack)
	uint8 bSetStunTimeToHitAnim : 1;
	UPROPERTY(EditAnywhere, Category = Attack)
	float StunTime;

	UPROPERTY(EditAnywhere, Category = PushPowar)
	float HitPushPowar;
	UPROPERTY(EditAnywhere, Category = PushPowar)
	float KnockDownPushPowar;
	UPROPERTY(EditAnywhere, Category = PushPowar)
	float BlockPushPowar;
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

	UPROPERTY(EditAnywhere, Category = Attack)
	TArray<FAttackInfo> AttackColInfos;
	


};
