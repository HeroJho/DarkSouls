// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Monster.h"

#include "Component/Collision/DK_CollisionManagerComponent.h"


ADK_Monster::ADK_Monster()
{

	CollisionManagerComponent = CreateDefaultSubobject<UDK_CollisionManagerComponent>("CollisionManagerComponent");

}


void ADK_Monster::BeginPlay()
{
	Super::BeginPlay();


}

void ADK_Monster::PostInitializeComponents()
{
	Super::PostInitializeComponents();


}

float ADK_Monster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{


	return 0.0f;
}


