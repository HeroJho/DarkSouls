// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimPlayer.h"

#include "Creature/DK_Player.h"


UDK_AnimPlayer::UDK_AnimPlayer()
{
}

void UDK_AnimPlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	PlayerOwner = Cast<ADK_Player>(GetOwningActor());

}

void UDK_AnimPlayer::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (PlayerOwner)
	{
		ChargePaworAcc = PlayerOwner->GetChargePowarTimeAcc();
		bIsCharging = PlayerOwner->GetIsCharging();
		InputDir = PlayerOwner->GetInputDir();
		InputDir.Normalize();
	}
	
}
