// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Creature/DK_Creature.h"
#include "DK_RamPage.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_RamPage : public ADK_Creature
{
	GENERATED_BODY()

public:
	ADK_RamPage();

protected:
	virtual void BeginPlay() override;


	// Skill Section
public:
	void Skill_Combo0();

protected:
	void BindFunction_Skill_Combo0();

	UFUNCTION()
	void BeginNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void EndNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void End_Skill_Combo0(FName NotifyName);

};
