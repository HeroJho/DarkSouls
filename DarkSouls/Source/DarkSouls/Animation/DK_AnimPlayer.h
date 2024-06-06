// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/DK_AnimCreature.h"
#include "DK_AnimPlayer.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_AnimPlayer : public UDK_AnimCreature
{
	GENERATED_BODY()
	
public:
	UDK_AnimPlayer();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	float ChargePaworAcc;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Player)
	uint8 bIsCharging : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Player)
	TObjectPtr<class ADK_Player> PlayerOwner;

};
