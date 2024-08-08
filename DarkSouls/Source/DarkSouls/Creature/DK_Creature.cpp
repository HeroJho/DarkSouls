// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/ArrowComponent.h"

#include "Component/Stat/DK_CreatureStatComponent.h"
#include "Component/Attack/DK_AttackComponent.h"
#include "UI/DK_WidgetComponent.h"
#include "Creature/Monster/DK_AIControllerBase.h"


ADK_Creature::ADK_Creature()
{
	// StatComponent
	CreatureStatComponent = CreateDefaultSubobject<UDK_CreatureStatComponent>(TEXT("CreatureStatComponent"));
	StatComponent = CreatureStatComponent;

	// AttackComponent
	AttackComponent = CreateDefaultSubobject<UDK_AttackComponent>(TEXT("AttackComponent"));
	
}


void ADK_Creature::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AIControllerBase = Cast<ADK_AIControllerBase>(NewController);
}

void ADK_Creature::BeginPlay()
{
	Super::BeginPlay();


	CreatureStatComponent->OnMaxGPDelegate.AddUObject(this, &ADK_Creature::StartGroggy);
	CreatureStatComponent->OnChangeHPDelegate.AddUFunction(WidgetComponent->GetWidget(), FName("UpdateBar"));
	OnOffScreenHPBar(true);
	OnOffHUDHPBar(true);

	CreatureStatComponent->ResetStat(RecoveryGPPerSec);
}

void ADK_Creature::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ADK_Creature::StartGroggy()
{
	KnockDown(5.f);
}

void ADK_Creature::DamagedByGPAttacked(int32 GPValue)
{
	CreatureStatComponent->IncreaseGP(GPValue);
}


void ADK_Creature::BeBlockedPerfectly(int32 GPValue)
{
	Super::BeBlockedPerfectly(GPValue);

	CreatureStatComponent->IncreaseGP(GPValue);

}

void ADK_Creature::SetIsKnockDown(bool bValue)
{
	Super::SetIsKnockDown(bValue);

	if (bValue)
	{
		AIControllerBase->SetStateAsFrozen();
	}
	else
	{
		AIControllerBase->SetStateAsAttacking(nullptr, true);
	}

}



bool ADK_Creature::CanKnockDown()
{
	//if (!Super::CanKnockDown())
	//	return false;

	//if (bIsKnockDown)
	//	return false;


	return true;
}


