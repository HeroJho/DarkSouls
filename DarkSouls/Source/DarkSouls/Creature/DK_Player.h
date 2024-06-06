// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/DK_Creature.h"
#include "InputActionValue.h"
#include "DK_Player.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Player : public ADK_Creature
{
	GENERATED_BODY()

public:
	ADK_Player();

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;


	// Input Section
protected:
	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

protected:
	FVector2D MoveInputDir;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaulMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> AttackAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> TargetLockAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> DodgeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> BlockAction;


	// Attack Section
public:
	FORCEINLINE float GetChargePowarTimeAcc() { return ChargePowarTimeAcc; }
	FORCEINLINE bool GetIsCharging() { return bIsCharging; }

	void CheckAttack_Notify();

protected:
	void ChargeAttack(const FInputActionValue& Value);
	void EndChargeAttack(const FInputActionValue& Value);

	void SmallAttack();
	void PowarAttack();
	void ResetChargeAttack();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> PowarAttackAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> SmallAttackAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float MaxChargePowerTime = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack)
	float PowarAttackTimeThreshould = 0.f;

	float ChargePowarTimeAcc = 0.f;
	bool bIsCharging = false;
	bool bIsHoldingAttackKey = false;
	bool bIsAttacking = false;


	// State Section
public:
	virtual void Stun(float StunTime, bool bSetAnimTime = false) override;
	

protected:
	virtual void EndStun() override;


	// LockOn Section
public:
	void LockTarget();
	void LockTarget(bool bIsLock);

protected:
	void LockTick();


protected:
	bool bIsTargetLockOn = false;
	TSoftObjectPtr<AActor> TargetLockedOn;

	UPROPERTY(EditAnywhere, Category = LockOn)
	float LockOnSmoothSpeed = 5.f;

	const float LockMaxDis = 3000.f;
	const float MaxAngle = 90.f;
	const float LockOnCameraLagSpeed = 3.f;
	const float LockOffCameraLagSpeed = 6.f;
	

	// Dodge Section
protected:
	virtual void Dodge() override;



	// Block Secion
public:
	virtual void Block() override;
	
	FORCEINLINE bool IsPerfectBlock() { return bIsPerfectBlock; }

protected:
	virtual void BlockAttack(AActor* Attacker, float PushBackPowar) override;

	void StartPerfectBlock();
	void EndPerfectBlock();

	void HitWeakBlock();


protected:	
	bool bIsPerfectBlock = false;
	FTimerHandle PerfectBlockTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Block, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BlockWeakHitMontage;



	// Condition Section
protected:
	bool CanMove();

	virtual bool CanAttack() override;
	virtual bool CanDodge() override;
	virtual bool CanBlock() override;
	virtual bool CanSmoothTurn() override;
	virtual bool CanStun() override;
	virtual bool CanKnockDown() override;
	virtual bool CanDamaged() override;
	

	// 상태에 들어가면 초기화해야할 작업들
// 공격 시,
	virtual void ResetInfoOnAttack() override;
	// 스턴 시, 
	virtual void ResetInfoOnStun() override;
	// 기절 시,
	virtual void ResetInfoOnKnockDown() override;
	// 회피 시,
	virtual void ResetInfoOnDodge() override;
	// 가드 시,
	virtual void ResetInfoOnBlock() override;
	// 가드 힛 시,
	virtual void ResetInfoOnHitBlock() override;

};
