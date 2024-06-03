// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimCreature.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Creature/DK_Creature.h"

UDK_AnimCreature::UDK_AnimCreature()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

}

void UDK_AnimCreature::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	Owner = Cast<ADK_Creature>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UDK_AnimCreature::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}

	if (Owner)
	{
		bIsStun = Owner->IsStun();
	}
	
}
