// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotify_PlayerAttackCheck.h"

#include "Creature/DK_Player.h"


void UDK_AnimNotify_PlayerAttackCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ADK_Player* Player = Cast<ADK_Player>(
			MeshComp->GetOwner());

		if (Player)
		{
			Player->CheckAttack_Notify();
		}
	}

}
