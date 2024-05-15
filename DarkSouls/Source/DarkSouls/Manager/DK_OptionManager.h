// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DK_OptionManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_OptionManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_OptionManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	FORCEINLINE float GetMouseSensitivity() { return MouseSensitivity; }

protected:
	UPROPERTY(EditAnywhere)
	float MouseSensitivity;

		
};
