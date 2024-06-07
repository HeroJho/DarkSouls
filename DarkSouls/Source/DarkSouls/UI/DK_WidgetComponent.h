// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DK_WidgetComponent.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
protected:
	virtual void InitWidget() override;
};
