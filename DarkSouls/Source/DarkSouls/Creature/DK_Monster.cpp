// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Monster.h"
#include "Components/ArrowComponent.h"

#include "Component/Stat/DK_MonsterStatComponent.h"
#include "UI/DK_WidgetComponent.h"


ADK_Monster::ADK_Monster()
{
	// StatComponent
	MonsterStatComponent = CreateDefaultSubobject<UDK_MonsterStatComponent>(TEXT("MonsterStatComponent"));
	StatComponent = MonsterStatComponent;

}


void ADK_Monster::BeginPlay()
{
	Super::BeginPlay();


	MonsterStatComponent->OnMaxGPDelegate.AddUObject(this, &ADK_Monster::StartGroggy);
	MonsterStatComponent->OnChangeHPDelegate.AddUFunction(WidgetComponent->GetWidget(), FName("UpdateBar"));
	OnOffScreenHPBar(true);
	OnOffHUDHPBar(true);

	MonsterStatComponent->ResetStat(RecoveryGPPerSec);
}

void ADK_Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ADK_Monster::StartGroggy()
{
	KnockDown(5.f);
}

void ADK_Monster::DamagedByGPAttacked(int32 GPValue)
{
	MonsterStatComponent->IncreaseGP(GPValue);
}


void ADK_Monster::BeBlockedPerfectly(int32 GPValue)
{
	Super::BeBlockedPerfectly(GPValue);

	MonsterStatComponent->IncreaseGP(GPValue);

}

bool ADK_Monster::CanDamaged()
{
	return true;
}

bool ADK_Monster::CanKnockDown()
{
	//if (!Super::CanKnockDown())
	//	return false;

	//if (bIsKnockDown)
	//	return false;


	return true;
}


