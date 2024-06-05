// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotify_EndKnockDown.h"

#include "Creature/DK_Creature.h"

void UDK_AnimNotify_EndKnockDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ADK_Creature* Creature = Cast<ADK_Creature>(MeshComp->GetOwner());
		if (Creature)
		{
			Creature->EndKnockDown_Notify();
		}
	}

}
