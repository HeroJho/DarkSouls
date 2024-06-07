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


	// HPBar
public:
	FORCEINLINE class UDK_SmoothBarWidget* GetHpBarWidget() { return HPBarWidget; }
	FORCEINLINE class UDK_SmoothBarWidget* GetSpBarWidget() { return SPBarWidget; }

protected:
	TObjectPtr<class UDK_SmoothBarWidget> HPBarWidget;
	TObjectPtr<class UDK_SmoothBarWidget> SPBarWidget;

};
