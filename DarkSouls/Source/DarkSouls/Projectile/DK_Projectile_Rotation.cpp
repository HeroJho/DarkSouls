// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DK_Projectile_Rotation.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADK_Projectile_Rotation::ADK_Projectile_Rotation()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ADK_Projectile_Rotation::BeginPlay()
{
	Super::BeginPlay();

}

void ADK_Projectile_Rotation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ProjectileMovementComponent->bSimulationEnabled == false)
		return;

	FRotator Rot = GetActorRotation();

	Rot.Pitch += DeltaTime * RotSpeed.X;
	Rot.Yaw += DeltaTime * RotSpeed.Z;
	Rot.Roll += DeltaTime * RotSpeed.Y;
	if (Rot.Pitch > 360.f)
		Rot.Pitch = 0.f;
	if (Rot.Yaw > 360.f)
		Rot.Yaw = 0.f;
	if (Rot.Roll > 360.f)
		Rot.Roll = 0.f;

	SetActorRotation(Rot);

}



