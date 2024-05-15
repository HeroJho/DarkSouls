// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DK_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_GameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	ADK_GameMode();

	virtual void StartPlay() override;


public:
	FORCEINLINE class UDK_OptionManager* GetOptionManager() { return OptionManager; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Manager)
	TObjectPtr<class UDK_OptionManager> OptionManager;

};
