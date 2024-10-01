// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/DK_Projectile_Base.h"
#include "DK_Projectile_Gravity.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Projectile_Gravity : public ADK_Projectile_Base
{
	GENERATED_BODY()
	
public:
	ADK_Projectile_Gravity();

protected:
	virtual void BeginPlay() override;


protected:
	virtual void RotateToTarget() override;

protected:
	UPROPERTY(EditAnywhere, Category = Projectile)
	float PitchDegree;

};
