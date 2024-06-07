// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DK_UserWidget.h"
#include "DK_SmoothBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_SmoothBarWidget : public UDK_UserWidget
{
	GENERATED_BODY()
	
public:
	UDK_SmoothBarWidget();

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void UpdateHpBar(uint32 IN_CurHP, uint32 IN_MaxHP);

protected:
	void TickHpBar();

protected:
	float HPAcc = 0.f;

	float CurHP = 0.f;
	float MaxHP = 0.f;

	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	FTimerHandle HpBarTimerHandle;

};
