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
	// 섹션마다 불리는 작업
	void BindSectionFunction_Skill_Combo0();
	UFUNCTION()
	void BeginSectionNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void EndSectionNotify_Skill_Combo0(FName NotifyName);
	UFUNCTION()
	void EndSection_Skill_Combo0(FName NotifyName);

	// 콤보 단위 작업
	void Interrupted_ComboSkill_Combo0();
	void End_ComboSkill_Combo0();


public:
	bool GroundSmash();

protected:
	void BindSectionFunction_GroundSmash();
	UFUNCTION()
	void BeginSectionNotify_GroundSmash(FName NotifyName);
	UFUNCTION()
	void EndSectionNotify_GroundSmash(FName NotifyName);
	UFUNCTION()
	void EndSection_GroundSmash(FName NotifyName);

	void Interrupted_ComboGroundSmash();
	void End_ComboGroundSmash();


public:
	bool JumpAttack();

protected:
	void BindSectionFunction_JumpAttack();
	UFUNCTION()
	void BeginSectionNotify_JumpAttack(FName NotifyName);
	UFUNCTION()
	void EndSectionNotify_JumpAttack(FName NotifyName);
	UFUNCTION()
	void EndSection_JumpAttack(FName NotifyName);

	void Interrupted_ComboJumpAttack();
	void End_ComboJumpAttack();

	void StartEndJumpAttackAnim();
	void EndPathJumpAttack();




protected:
	UPROPERTY(EditAnywhere, Category = JumpAttack)
	TObjectPtr<UCurveFloat> JumpAttackCurve;
	UPROPERTY(EditAnywhere, Category = JumpAttack)
	TObjectPtr<UParticleSystem> JumpAttackParticle;
	UPROPERTY(EditAnywhere, Category = JumpAttack)
	float JumpAttackSpeed = 0.5f;
};
