// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimNotifyState_Rotation.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Creature/DK_Object.h"
#include "Creature/Monster/DK_AIControllerBase.h"

UDK_AnimNotifyState_Rotation::UDK_AnimNotifyState_Rotation()
{
	RotationSpeed = 0.f;
}

void UDK_AnimNotifyState_Rotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ADK_Object* CharacterOwner = Cast<ADK_Object>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	UCharacterMovementComponent* Movement = CharacterOwner->GetCharacterMovement();
	if (!IsValid(Movement))
	{
		return;
	}

	ADK_AIControllerBase* AIController = Cast<ADK_AIControllerBase>(CharacterOwner->GetController());
	if (!IsValid(AIController))
	{
		return;
	}

	Movement->RotationRate = FRotator(0.f, RotationSpeed, 0.f);
	Movement->bUseControllerDesiredRotation = true;
	Movement->bOrientRotationToMovement = false;
	AIController->SetFocusTarget();

}

void UDK_AnimNotifyState_Rotation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

}

void UDK_AnimNotifyState_Rotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);


	ADK_Object* CharacterOwner = Cast<ADK_Object>(MeshComp->GetOwner());
	if (!IsValid(CharacterOwner))
	{
		return;
	}

	UCharacterMovementComponent* Movement = CharacterOwner->GetCharacterMovement();
	if (!IsValid(Movement))
	{
		return;
	}

	ADK_AIControllerBase* AIController = Cast<ADK_AIControllerBase>(CharacterOwner->GetController());
	if (!IsValid(AIController))
	{
		return;
	}

	Movement->RotationRate = FRotator(0.f, 360.f, 0.f);
	Movement->bUseControllerDesiredRotation = false;
	Movement->bOrientRotationToMovement = true;
	AIController->ClearFocus(EAIFocusPriority::Gameplay);
	AIController->SetControlRotation(CharacterOwner->GetActorRotation());
}
