// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DK_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ADK_PlayerController();

protected:
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

public:
	FORCEINLINE class UDK_HUDWidget* GetHUDWidget() { return HUDWidget; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UDK_HUDWidget> HUDWidgetClass;
	
	TObjectPtr<class UDK_HUDWidget> HUDWidget;

};
