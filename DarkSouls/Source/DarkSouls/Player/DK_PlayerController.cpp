// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DK_PlayerController.h"

#include "UI/DK_HUDWidget.h"


ADK_PlayerController::ADK_PlayerController()
{
}


void ADK_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	HUDWidget = CreateWidget<UDK_HUDWidget>(this, HUDWidgetClass);
	if (HUDWidget)
	{
		HUDWidget->AddToViewport();
	}

}

void ADK_PlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}
