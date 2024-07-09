#pragma once

#include "CoreMinimal.h"
#include "Enum/E_Damage.h"
#include "S_DamageInfo.generated.h"


USTRUCT(BlueprintType)
struct FS_DamageInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse DamageResponse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bShouldForceInterrupt : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bShouldDamageInvincible : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanBeBlocked : 1 = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 bCanBeParried : 1 = false;

};
