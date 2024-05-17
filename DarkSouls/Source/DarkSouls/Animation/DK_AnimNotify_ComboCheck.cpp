// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotify_ComboCheck.h"

#include "Component/Combo/DK_ComboComponent.h"


void UDK_AnimNotify_ComboCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		UDK_ComboComponent* ComboComponent = Cast<UDK_ComboComponent>(
			MeshComp->GetOwner()->GetComponentByClass(UDK_ComboComponent::StaticClass()));

		if (ComboComponent)
		{
			ComboComponent->ComboCheck();
		}
	}
}