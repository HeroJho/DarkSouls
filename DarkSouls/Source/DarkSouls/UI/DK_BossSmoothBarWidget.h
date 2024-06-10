// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DK_SmoothBarWidget.h"
#include "DK_BossSmoothBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_BossSmoothBarWidget : public UDK_SmoothBarWidget
{
	GENERATED_BODY()

public:
	UDK_BossSmoothBarWidget();

protected:
	virtual void NativeConstruct() override;

public:
	virtual void UpdateBar(uint32 IN_Cur, uint32 IN_Max) override;
	void SetBossName(FText Name);

protected:
	UPROPERTY()
	TObjectPtr<class UTextBlock> BossName;

};
