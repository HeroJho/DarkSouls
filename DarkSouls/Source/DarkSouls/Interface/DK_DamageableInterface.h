// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Struct/S_DamageInfo.h"
#include "DK_DamageableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDK_DamageableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKSOULS_API IDK_DamageableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser) = 0;

};
