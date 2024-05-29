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


	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<class UCameraComponent> FollowCamera;


	// Input Section
protected:
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


	void ShoulderMove(const FInputActionValue& Value);
	void ShoulderLook(const FInputActionValue& Value);

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

	UFUNCTION()
	void OnWeaponOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeaponOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


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

	UPROPERTY()
	TObjectPtr<class UCapsuleComponent> WeaponCapsuleComponentTemp;

};
