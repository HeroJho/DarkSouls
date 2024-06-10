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
	PlayerStatComponent->AddChangeHPDelegateFunc(HUDWidget, FName("UpdateHpBar"));
	PlayerStatComponent->AddChangeSPDelegateFunc(HUDWidget, FName("UpdateSpBar"));
	PlayerStatComponent->AddChangeTPDelegateFunc(HUDWidget, FName("UpdateTpBar"));

	PlayerStatComponent->AddChangeMaxHPDelegateFunc(HUDWidget, FName("UpdateMaxHpBar"));
	PlayerStatComponent->AddChangeMaxSPDelegateFunc(HUDWidget, FName("UpdateMaxSpBar"));

}






