// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DK_Projectile_Base.generated.h"

DECLARE_DELEGATE_TwoParams(FOnColHitTwoDelegete, AActor*, const FHitResult& Hit)

struct ProjectileOption
{
	ProjectileOption() {};
	ProjectileOption(AActor* InTarget, float InLifeTime, float InHomingAcceleration, float InSpeed, float InGravity, 
		bool InbRotateToTarget, bool InbSimulating)
		: Target(InTarget), LifeTime(InLifeTime), HomingAcceleration(InHomingAcceleration), Speed(InSpeed), Gravity(InGravity),
		bRotateToTarget(InbRotateToTarget), bSimulating(InbSimulating)
	{};

	AActor* Target = nullptr;
	float LifeTime = 0.f;
	float HomingAcceleration = 0.f;
	float Speed = 0.f;
	float Gravity = 0.f;
	bool bRotateToTarget = false;
	bool bSimulating = false;
};

UCLASS()
class DARKSOULS_API ADK_Projectile_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	ADK_Projectile_Base();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


public:
	void Init(ProjectileOption Option);

	void SimulatingProjectile();

	UFUNCTION()
	void DestroyProjectile(FVector HitPos = FVector::ZeroVector);

protected:
	virtual void RotateToTarget();
	


	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UProjectileMovementComponent> ProjectileMovementComponent;
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UBoxComponent> BoxCollision;
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UStaticMeshComponent> StaticMesh;
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UArrowComponent> Arrow;



	// RunTime Option
	TWeakObjectPtr<AActor> TargetActor;
	float Speed;
	float Gravity;
	float HomingAcceleration;
	float LifeTime;
	bool bSimulating;

	// Static Option
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UParticleSystem> ImpactEffect;
	UPROPERTY(EditAnywhere, Category = Projectile)
	FRotator RotSpeed;


	FTimerHandle LifeTimerHandle;

	// Delegate
public:
	FOnColHitTwoDelegete OnProjectileImpact;

};
