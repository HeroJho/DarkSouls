// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/DK_Monster.h"
#include "DK_Greater_Spider.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_Greater_Spider : public ADK_Monster
{
	GENERATED_BODY()
	
public:
	ADK_Greater_Spider();

protected:
	virtual void BeginPlay() override;


protected:


};
