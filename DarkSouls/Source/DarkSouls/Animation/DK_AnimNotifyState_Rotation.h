// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DK_AnimNotifyState_Rotation.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_AnimNotifyState_Rotation : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	UDK_AnimNotifyState_Rotation();

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;



protected:
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

};
