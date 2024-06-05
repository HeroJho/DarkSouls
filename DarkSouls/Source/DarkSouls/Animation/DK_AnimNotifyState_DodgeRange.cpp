// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotifyState_DodgeRange.h"

#include "Creature/DK_Creature.h"


void UDK_AnimNotifyState_DodgeRange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		ADK_Creature* Creature = Cast<ADK_Creature>(MeshComp->GetOwner());
		if (Creature)
		{
			Creature->BeginDodgeSkip_Notify();
		}
	}

}

void UDK_AnimNotifyState_DodgeRange::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);


}

void UDK_AnimNotifyState_DodgeRange::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp && MeshComp->GetOwner())
	{
		ADK_Creature* Creature = Cast<ADK_Creature>(MeshComp->GetOwner());
		if (Creature)
		{
			Creature->EndDodgeSkip_Notify();
		}
	}

}