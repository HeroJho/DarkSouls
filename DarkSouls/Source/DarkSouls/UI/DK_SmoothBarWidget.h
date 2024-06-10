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
	virtual void UpdateBar(uint32 IN_Cur, uint32 IN_Max);

	FORCEINLINE class UProgressBar* GetProgressBar() { return ProgressBar; }

protected:
	void TickBar();


protected:
	float Acc = 0.f;

	float Cur = 0.f;
	float Max = 0.f;

	UPROPERTY()
	TObjectPtr<class UProgressBar> ProgressBar;

	FTimerHandle BarTimerHandle;

};
