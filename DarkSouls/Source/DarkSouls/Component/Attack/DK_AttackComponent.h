// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/S_DamageInfo.h"
#include "DK_AttackComponent.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnNoParDelegate);



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
	UFUNCTION(BlueprintCallable)
	void AOEDamage(FVector SpawnLocation, float Radius, FS_DamageInfo DamageInfo, bool bIsRenderDebug = false);


	// Jump
public:
	bool JumpToAttackTarget(AActor* Target, FS_JumpAttackInfo JumpAttackInfo);
	bool JumpToPos(FVector Pos, FS_JumpAttackInfo JumpAttackInfo);


protected:
	UFUNCTION()
	void JumpTick(UCurveFloat* Curve, TArray<FVector> Poss, float EndAnimLength, float JumpSpeed, float EndAnimPlayRatio);

protected:
	FTimerHandle JumpTimerHandle;
	float JumpDeltaTimeAcc = 0.f;

	bool bTriggerStartEndAnim = false;

	// Delegate
public:
	FOnNoParDelegate Delegate_StartEndAnim;
	FOnNoParDelegate Delegate_EndJump;

};
