// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimCreature.h"
#include "AIController.h"

#include "Creature/DK_Creature.h"

UDK_AnimCreature::UDK_AnimCreature()
{
	bIsStrafe = false;
}

void UDK_AnimCreature::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Owner)
	{
		CreatureOwner = Cast<ADK_Creature>(Owner);
		AIController = Cast<AAIController>(Owner->GetController());
	}

}

void UDK_AnimCreature::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (Owner)
	{
		// 멈출때 Velocity가 0이 되면서 허리를 꼬는 문제 발생 방지
		if (!bIsIdle)
		{
			RotValDisDegree = CalculateDirection(Velocity, Owner->GetActorRotation());
		}

	}

	if (CreatureOwner.IsValid())
	{
		bIsStrafe = CreatureOwner->GetIsStrafe();
	}

}
