// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DK_PlayerController.h"

#include "Game/DK_GameMode.h"
#include "Manager/DK_UIManager.h"

ADK_PlayerController::ADK_PlayerController()
{
}


void ADK_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());
	GameMode->GetUIManager()->MakeHUD(this);

}

