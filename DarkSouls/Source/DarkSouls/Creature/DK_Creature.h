// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DK_Creature.generated.h"

UCLASS()
class DARKSOULS_API ADK_Creature : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADK_Creature();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Common
public:
	void AddImpulse(FVector Dir, float Powar);

	FORCEINLINE float GetBlockSpeed() { return BlockSpeed; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float NormalSpeed = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float BlockSpeed = 150.f;






	// UI
public:
	void OnOffSceenHPBar(bool bIsOn);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TObjectPtr<class UDK_WidgetComponent> WidgetComponent;







	// Stat
public:


protected:
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Stat)
	TSoftObjectPtr<class UDK_StatComponent> StatComponent;






	// Combo && Attack
public:
	UFUNCTION(BlueprintCallable)
	void Attack();

	FORCEINLINE bool IsInAttackRange() { return bIsInAttackRange; }

	void BeginAttackRange_Notify();
	void EndAttackRange_Notify();

	void BeginColRange_Notify();
	void EndColRange_Notify();

	FAttackDamagedInfo GetCurrentAttackInfos();


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combo)
	TObjectPtr<class UDK_ComboComponent> ComboComponent;

	bool bIsInAttackRange = false;







	// Collsion
public:
	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);*/

	virtual void OnDamaged(const FAttackDamagedInfo& AttackDamagedInfo, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	TObjectPtr<class UDK_CollisionManagerComponent> CollisionManagerComponent;






	// Stun
public:
	FORCEINLINE bool IsStun() { return bIsStun; }
	virtual void Stun(float StunTime, bool bSetAnimTime = false);

	FORCEINLINE bool IsKnockDown() { return bIsKnockDown; }
	FORCEINLINE bool IsPlayEndKnockDown() { return bIsPlayEndKnockDown; }
	virtual void KnockDown(float KnockDownTime);
	void EndKnockDown_Notify();
	
protected:
	virtual void EndStun();

	virtual void EndKnockDown();


protected:
	FTimerHandle StunTimerHandle;
	bool bIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stun, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;


	FTimerHandle KnockDownTimerHandle;
	bool bIsKnockDown = false;
	bool bIsPlayEndKnockDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stun, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> StartKnockDownMontage;






	// SmoothTurn
public:
	UFUNCTION(BlueprintCallable)
	void SmoothTurnByCallOnce(FVector InDestPos, float TurnSpeed);
	void StopSmoothTurn();

protected:
	void SmoothTurnTick();

protected:
	bool bIsSmoothTurn = false;
	float SmoothTurnSpeed = 0.f;
	FVector DestPos;






	// Dodge
public:
	virtual void Dodge();
	void EndDoge(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void BeginDodgeSkip_Notify();
	virtual void EndDodgeSkip_Notify();


protected:
	bool bIsDodge = false;
	bool bCanDodgeSkip = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DodgeMontage;






	// Block
public:
	FORCEINLINE bool IsBlock() { return bIsBlock; }
	FORCEINLINE FVector2D GetBlockMoveDir() { return BlockMoveDir; }

	virtual void Block();
	void EndBlock();

	void HitBlock();
	void EndHitBlock();

protected:
	virtual void BlockAttack(AActor* Attacker, float PushBackPowar);

	virtual bool CheckBlock(FVector AttackerPos);
	



protected:
	bool bIsBlock = false;
	bool bIsHitBlock = false;
	FVector2D BlockMoveDir;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Block, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BlockHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Block, Meta = (AllowPrivateAccess = "true"))
	float HitTime = 0.f;
	
	FTimerHandle HitBlockTimerHandle;




	// Condition
protected:
	virtual bool CanAttack();
	virtual bool CanDodge();
	virtual bool CanSmoothTurn();
	virtual bool CanStun();
	virtual bool CanKnockDown();
	virtual bool CanDamaged();
	virtual bool CanBlock();


	// 상태에 들어가면 초기화해야할 작업들
	// 공격 시,
	virtual void ResetInfoOnAttack();
	// 스턴 시, 
	virtual void ResetInfoOnStun();
	// 기절 시,
	virtual void ResetInfoOnKnockDown();
	// 회피 시,
	virtual void ResetInfoOnDodge();
	// 가드 시,
	virtual void ResetInfoOnBlock();
	// 가드 힛 시,
	virtual void ResetInfoOnHitBlock();

};
