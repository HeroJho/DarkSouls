// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DK_UIManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_UIManager : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDK_UIManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// HUD Section
public:
	FORCEINLINE class UDK_HUDWidget* GetHUD() { return HUDWidget; }

public:
	void MakeHUD(APlayerController* PlayerController);
	void BindEventForHUD(class UDK_PlayerStatComponent* PlayerStatComponent);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UDK_HUDWidget> HUDWidgetClass;

	TObjectPtr<class UDK_HUDWidget> HUDWidget;


};
