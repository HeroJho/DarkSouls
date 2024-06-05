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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// State
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float NormalSpeed = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float BlockSpeed = 20.f;




	// Combo && Attack
public:
	UFUNCTION(BlueprintCallable)
	void Attack();

	FORCEINLINE bool IsInAttackRange() { return bIsInAttackRange; }

	void BeginAttackRange_Notify();
	void EndAttackRange_Notify();

	void BeginColRange_Notify();
	void EndColRange_Notify();

	void GetCurrentAttackInfos(float& OUT_Damage, bool& OUT_bIsDown, bool& OUT_bSetStunTimeToHitAnim, float& OUT_StunTime);


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combo)
	TObjectPtr<class UDK_ComboComponent> ComboComponent;

	bool bIsInAttackRange = false;







	// Collsion
public:
	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);*/

	virtual void OnDamaged(float DamageAmount, bool bIsDown, bool bSetStunTimeToHitAnim, float StunTime, AActor* DamageCauser);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	TObjectPtr<class UDK_CollisionManagerComponent> CollisionManagerComponent;







	// Stun
public:
	FORCEINLINE bool IsStun() { return bIsStun; }
	virtual void Stun(float StunTime);

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
	virtual void Block();
	void EndBlock();

	FORCEINLINE bool IsBlock() { return bIsBlock; }


protected:
	bool bIsBlock = false;


	// Condition
protected:
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


};
