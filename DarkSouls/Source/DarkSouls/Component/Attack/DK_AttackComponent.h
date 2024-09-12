// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/S_DamageInfo.h"
#include "DK_AttackComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNoParDelegate);


USTRUCT(BlueprintType)
struct FS_JumpAttackInfo
{
	GENERATED_USTRUCT_BODY()

public:
	FS_JumpAttackInfo();
	FS_JumpAttackInfo(class UCurveFloat* In_Curve, float In_JumpSpeed, float In_PredictTime,
		float In_Arc, float In_MinDisRange, float In_MaxDisRange, float In_MinArc, float In_MaxArc,
		bool In_bIsFrontTarget, float In_FrontDis, bool In_bRenderDebug);


public:
	class UCurveFloat* Curve;
	float JumpSpeed;

	float PredictTime;

	float Arc;
	float MinDisRange;
	float MaxDisRange;
	float MinArc;
	float MaxArc;

	bool bIsFrontTarget;
	float FrontDis;

	bool bRenderDebug;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_AttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDK_AttackComponent();


	// Common
protected:
	virtual void BeginPlay() override;


protected:
	TWeakObjectPtr<ACharacter> CharacterOwner;


	// AOE
public:
	void AOEDamage(FVector SpawnLocation, float Radius, FS_DamageInfo DamageInfo, bool bIsRenderDebug = false);


	// Jump
public:
	bool JumpToAttackTarget(AActor* Target, FS_JumpAttackInfo JumpAttackInfo);

protected:
	UFUNCTION()
	void JumpTick(UCurveFloat* Curve, TArray<FVector> Poss, float EndAnimLength, float JumpSpeed);

protected:
	FTimerHandle JumpTimerHandle;
	float JumpDeltaTimeAcc = 0.f;


	// Delegate
public:
	FOnNoParDelegate Delegate_EndJump;

};
