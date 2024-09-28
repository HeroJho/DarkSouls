// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DK_Projectile_Base.generated.h"

DECLARE_DELEGATE_TwoParams(FOnColHitTwoDelegete, AActor*, const FHitResult&)

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

private:
	void RotateToTarget();
	
	
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* OnComponentHit, UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);


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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Test)
	TWeakObjectPtr<class AActor> TargetActor;

	UPROPERTY(EditAnywhere, Category = Test)
	float Speed;
	UPROPERTY(EditAnywhere, Category = Test)
	float Gravity;
	UPROPERTY(EditAnywhere, Category = Test)
	bool bIsHoming;

	// Static Option
	UPROPERTY(EditAnywhere, Category = Projectile)
	TObjectPtr<class UParticleSystem> ImpactEffect;



	// Delegate
public:
	FOnColHitTwoDelegete OnProjectileImpact;

};
