// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Monster.h"
#include "Components/ArrowComponent.h"

#include "Component/Stat/DK_MonsterStatComponent.h"



ADK_Monster::ADK_Monster()
{
	// StatComponent
	MonsterStatComponent = CreateDefaultSubobject<UDK_MonsterStatComponent>(TEXT("MonsterStatComponent"));
	StatComponent = MonsterStatComponent;

}


void ADK_Monster::BeginPlay()
{
	Super::BeginPlay();


}

void ADK_Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}


