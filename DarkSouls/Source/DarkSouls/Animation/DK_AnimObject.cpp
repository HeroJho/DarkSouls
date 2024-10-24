// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimObject.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Creature/DK_Object.h"


UDK_AnimObject::UDK_AnimObject()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;

}

void UDK_AnimObject::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	Owner = Cast<ADK_Object>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
		BlockSpeed = Owner->GetBlockSpeed();
	}
}

void UDK_AnimObject::NativeUpdateAnimation(float DeltaSeconds)
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
		bIsKnockDown = Owner->IsKnockDown();
		bIsBlock = Owner->IsBlock();
		bIsSmallHittedTrigger = Owner->GetSmallHittedTrigger();
	}
	
}
