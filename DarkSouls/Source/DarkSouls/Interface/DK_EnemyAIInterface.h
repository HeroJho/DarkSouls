// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enum/E_MovementSpeed.h"
#include "DK_EnemyAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDK_EnemyAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOULS_API IDK_EnemyAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetStrafe(bool bValue) = 0;
	virtual float SetMovementSpeed(EMovementSpeed MovementSpeed) = 0;

};
