// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DK_GameMode.h"

ADK_GameMode::ADK_GameMode()
{
	OptionManager = CreateDefaultSubobject<UDK_OptionManager>(TEXT("OptionManager"));

}

void ADK_GameMode::StartPlay()
{
	Super::StartPlay();

}

