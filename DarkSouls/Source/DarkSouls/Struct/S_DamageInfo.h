#pragma once

#include "CoreMinimal.h"
#include "Enum/E_Damage.h"
#include "S_DamageInfo.generated.h"



USTRUCT(BlueprintType)
struct FS_JumpAttackInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FS_JumpAttackInfo();
	FS_JumpAttackInfo(class UCurveFloat* InCurve, float InJumpSpeed, float InPredictTime,
		float InArc, float InMinDisRange, float InMaxDisRange, float InMinArc, float InMaxArc,
		bool InbIsFrontTarget, float InFrontDis, float InEndAnimPlayRatio, bool InbRenderDebug);


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	class UCurveFloat* Curve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float JumpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float PredictTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float Arc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float MinDisRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float MaxDisRange;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float MinArc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float MaxArc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	bool bIsFrontTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float FrontDis;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	float EndAnimPlayRatio;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	bool bRenderDebug;
};




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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Jump)
	FS_JumpAttackInfo JumpAttackInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Col)
	TArray<FString> AttackColInfos;

};


