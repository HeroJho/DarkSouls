// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DK_UIManager.h"

#include "UI/DK_HUDWidget.h"
#include "Component/Stat/DK_PlayerStatComponent.h"



// Sets default values for this component's properties
UDK_UIManager::UDK_UIManager()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_UIManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




void UDK_UIManager::MakeHUD(APlayerController* PlayerController)
{
	HUDWidget = CreateWidget<UDK_HUDWidget>(PlayerController, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

}

void UDK_UIManager::BindEventForHUD(UDK_PlayerStatComponent* PlayerStatComponent)
{
	PlayerStatComponent->OnChangeHPDelegate.AddUFunction(HUDWidget, FName("UpdateHpBar"));
	PlayerStatComponent->OnChangeSPDelegate.AddUFunction(HUDWidget, FName("UpdateSpBar"));
	PlayerStatComponent->OnChangeTPDelegate.AddUFunction(HUDWidget, FName("UpdateTpBar"));

	PlayerStatComponent->OnChangeMaxHPDelegate.AddUFunction(HUDWidget, FName("UpdateMaxHpBar"));
	PlayerStatComponent->OnChangeMaxSPDelegate.AddUFunction(HUDWidget, FName("UpdateMaxSpBar"));

}






