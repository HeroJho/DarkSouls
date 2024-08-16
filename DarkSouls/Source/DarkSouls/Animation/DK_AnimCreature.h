// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/DK_AnimObject.h"
#include "DK_AnimCreature.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_AnimCreature : public UDK_AnimObject
{
	GENERATED_BODY()
	
public:
	UDK_AnimCreature();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	TWeakObjectPtr<class ADK_Creature> CreatureOwner;
	TWeakObjectPtr<class AAIController> AIController;
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsStrafe : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	float RotValDisDegree;

};
