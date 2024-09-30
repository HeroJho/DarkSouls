// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/DK_Projectile_Base.h"
#include "DK_Projectile_Rotation.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Projectile_Rotation : public ADK_Projectile_Base
{
	GENERATED_BODY()
	
public:
	ADK_Projectile_Rotation();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


protected:
	UPROPERTY(EditAnywhere)
	FVector RotSpeed;

};
