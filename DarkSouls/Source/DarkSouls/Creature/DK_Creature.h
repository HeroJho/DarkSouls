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



	// Combo && Attack
public:
	UFUNCTION(BlueprintCallable)
	void Attack();

	FORCEINLINE bool IsInAttackRange() { return bIsInAttackRange; }

	void BeginAttackRange_Notify();
	void EndAttackRange_Notify();

	void BeginColRange_Notify();
	void EndColRange_Notify();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combo)
	TObjectPtr<class UDK_ComboComponent> ComboComponent;

	bool bIsInAttackRange = false;


	// Collsion
public:
	/*virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser);*/

	virtual void OnDamaged(float DamageAmount, float StunTime, AActor* DamageCauser, bool bIsDown = false);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	TObjectPtr<class UDK_CollisionManagerComponent> CollisionManagerComponent;


	// State
public:
	FORCEINLINE bool IsStun() { return bIsStun; }

	virtual void Stun(float StunTime);
	
protected:
	virtual void EndStun();

	// 스턴 시, 초기화해야할 작업들
	virtual void ResetInfoOnStun();

protected:
	FTimerHandle StunTimerHandle;
	bool bIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;


};
