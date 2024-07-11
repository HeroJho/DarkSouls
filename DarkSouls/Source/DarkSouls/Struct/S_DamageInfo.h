#pragma once

#include "CoreMinimal.h"
#include "Enum/E_Damage.h"
#include "S_DamageInfo.generated.h"


USTRUCT(BlueprintType)
struct FS_DamageInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FS_DamageInfo();
	FS_DamageInfo(int32 InAmount, EDamageType InDamageType, EDamageResponse InDamageResponse, 
		uint8 InbShouldForceInterrupt, uint8 InbShouldDamageInvincible, uint8 InbCanBeBlocked, uint8 InbCanBeParried);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	EDamageResponse DamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint8 bShouldForceInterrupt : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint8 bShouldDamageInvincible : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint8 bCanBeBlocked : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Damage)
	uint8 bCanBeParried : 1 = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
	int32 GPValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
	float StunTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
	float HitKnockBackPowar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Hit)
	float BlockKnockBackPowar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Col)
	TArray<FString> AttackColInfos;

};
