// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/S_DamageInfo.h"
#include "DK_StatComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnChangeDelegate, int32, int32);
DECLARE_MULTICAST_DELEGATE(FOnZeroDelegate);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnBlockDelegate, bool, float/*KnockBackPowar*/, AActor*);
DECLARE_MULTICAST_DELEGATE_FiveParams(FOnDamageResponse, EDamageResponse, int32/*GPValue*/, float/*StunTime*/, float/*KnockBackPowar*/, AActor*);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_StatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDK_StatComponent();

protected:
	virtual void BeginPlay() override;


	// Stat
public:
	virtual void ResetStat();
	virtual void BroadcastStat();

	FORCEINLINE bool GetIsDead() { return bIsDead; }

protected:
	UPROPERTY(EditAnywhere, Category = StatData)
	TObjectPtr<class UDK_StatDataAsset> StatData;

	int32 MaxHP = 100;
	int32 CurHP = 0;
	bool bIsDead = false;




	// TakeDamage
public:
	FORCEINLINE bool GetIsInterruptible() { return bIsInterruptible; }
	FORCEINLINE bool GetIsInvincible() { return bIsInvincible; }
	FORCEINLINE bool GetIsBlocking() { return bIsBlocking; }
	FORCEINLINE bool GetIsDodgSkip() { return bIsDodgSkip; }

	FORCEINLINE void SetIsInterruptible(bool bValue) { bIsInterruptible = bValue; }
	FORCEINLINE void SetIsInvincible(bool bValue) { bIsInvincible = bValue; }
	FORCEINLINE void SetIsBlocking(bool bValue) { bIsBlocking = bValue; }
	FORCEINLINE void SetIsDodgeSkip(bool bValue) { bIsDodgSkip = bValue; }

public:
	bool TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser);

protected:
	void IncreaseHP(int32 Value);

	bool CheckBlock(FVector AttackerPos);


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageOption)
	uint8 bIsInterruptible : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageOption)
	uint8 bIsInvincible : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageOption)
	uint8 bIsBlocking : 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DamageOption)
	uint8 bIsDodgSkip : 1;



	// Delegate
public:
	FOnChangeDelegate OnChangeHPDelegate;

	FOnZeroDelegate OnDeathDelegate;
	FOnBlockDelegate OnBlockDelegate;
	FOnDamageResponse OnDamageResponseDelegate;
	FOnZeroDelegate OnDodgSkipDelegate;

};
