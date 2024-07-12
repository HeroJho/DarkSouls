// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotify_EndKnockDown.h"

#include "Creature/DK_Object.h"

void UDK_AnimNotify_EndKnockDown::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ADK_Object* Creature = Cast<ADK_Object>(MeshComp->GetOwner());
		if (Creature)
		{
			Creature->EndKnockDown_Notify();
		}
	}

}
