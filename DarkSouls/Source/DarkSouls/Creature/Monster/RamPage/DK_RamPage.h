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
	bool Skill_Combo0();

protected:
	void BindFunction_Skill_Combo0();

	UFUNCTION()
	void BeginNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void EndNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void End_Skill_Combo0(FName NotifyName);



public:
	bool GroundSmash();

protected:
	void BindFunction_GroundSmash();

	UFUNCTION()
	void BeginNotify_GroundSmash(FName NotifyName);
	UFUNCTION()
	void EndNotify_GroundSmash(FName NotifyName);
	UFUNCTION()
	void End_GroundSmash(FName NotifyName);



public:
	bool JumpAttack();

protected:
	void BindFunction_JumpAttack();

	UFUNCTION()
	void BeginNotify_JumpAttack(FName NotifyName);
	UFUNCTION()
	void EndNotify_JumpAttack(FName NotifyName);
	UFUNCTION()
	void End_JumpAttack(FName NotifyName);

protected:
	UPROPERTY(EditAnywhere, Category = JumpCurve)
	TObjectPtr<UCurveFloat> JumpAttackCurve;

};
