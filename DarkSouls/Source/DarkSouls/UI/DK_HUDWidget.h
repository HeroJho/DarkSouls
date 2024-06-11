// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/DK_UserWidget.h"
#include "DK_HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_HUDWidget : public UDK_UserWidget
{
	GENERATED_BODY()

public:
	UDK_HUDWidget();

protected:
	virtual void NativeConstruct() override;


public:
	UFUNCTION()
	void UpdateHpBar(uint32 IN_Cur, uint32 IN_Max);
	UFUNCTION()
	void UpdateSpBar(uint32 IN_Cur, uint32 IN_Max);
	UFUNCTION()
	void UpdateTpBar(uint32 IN_Cur, uint32 IN_Max);

	UFUNCTION()
	void UpdateMaxHpBar(uint32 IN_Cur, uint32 IN_Max);
	UFUNCTION()
	void UpdateMaxSpBar(uint32 IN_Cur, uint32 IN_Max);

	bool MakeBossHpBar(class UDK_SmoothBarWidget** OUT_BossHpBar, FText BossName);
	void RemoveBossHpBar(class UDK_SmoothBarWidget* BossHpBar);

protected:
	TObjectPtr<class UDK_SmoothBarWidget> HPBarWidget;
	TObjectPtr<class UDK_SmoothBarWidget> SPBarWidget;
	TObjectPtr<class UDK_SmoothBarWidget> TPBarWidget;

	UPROPERTY(EditAnywhere, Category = BossHpBar)
	TSubclassOf<class UDK_SmoothBarWidget> BossHpBarWidgetClass;
	UPROPERTY(EditAnywhere, Category = BossHpBar)
	int32 MaxBossHpBarCnt = 4;

	TObjectPtr<class UVerticalBox> BossHpList;
	FVector2D BossHpListInitPos;

};
