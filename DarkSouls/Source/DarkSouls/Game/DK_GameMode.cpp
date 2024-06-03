// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/DK_GameMode.h"

#include "Manager/DK_OptionManager.h"
#include "Manager/DK_ToolManager.h"


ADK_GameMode::ADK_GameMode()
{
	OptionManager = CreateDefaultSubobject<UDK_OptionManager>(TEXT("OptionManager"));
	ToolManager = CreateDefaultSubobject<UDK_ToolManager>(TEXT("ToolManager"));
}

void ADK_GameMode::StartPlay()
{
	Super::StartPlay();

}

