// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/Stat/DK_CreatureStatComponent.h"
#include "Component/Attack/DK_AttackComponent.h"
#include "Component/AdaptiveGenNum/DK_AdaptiveGenNumComponent.h"
#include "UI/DK_WidgetComponent.h"
#include "Creature/Monster/DK_AIControllerBase.h"


ADK_Creature::ADK_Creature()
{
	// StatComponent
	CreatureStatComponent = CreateDefaultSubobject<UDK_CreatureStatComponent>(TEXT("CreatureStatComponent"));
	StatComponent = CreatureStatComponent;

	// AttackComponent
	AttackComponent = CreateDefaultSubobject<UDK_AttackComponent>(TEXT("AttackComponent"));

	// AdaptiveGenNumComponent
	AdaptiveGenNumComponent = CreateDefaultSubobject<UDK_AdaptiveGenNumComponent>(TEXT("AdaptiveGenNumComponent"));

	IdleSpeed = 0.f;
	WalkingSpeed = 150.f;
	JoggingSpeed = 250.f;
	SprintingSpeed = 400.f;
	
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








void ADK_Creature::SetStrafe(bool bValue)
{
	bIsStrafe = bValue;
}

float ADK_Creature::SetMovementSpeed(EMovementSpeed MovementSpeed)
{
	switch (MovementSpeed)
	{
	case EMovementSpeed::Idle:
		GetCharacterMovement()->MaxWalkSpeed = IdleSpeed;
		return IdleSpeed;
	case EMovementSpeed::Walking:
		GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		return WalkingSpeed;
	case EMovementSpeed::Jogging:
		GetCharacterMovement()->MaxWalkSpeed = JoggingSpeed;
		return JoggingSpeed;
	case EMovementSpeed::Sprinting:
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
		return SprintingSpeed;
	default:
		return 0.0f;
	}

	return 0.0f;
}


