// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/DK_AnimCreature.h"
#include "DK_AnimRamPage.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_AnimRamPage : public UDK_AnimCreature
{
	GENERATED_BODY()
	
public:
	UDK_AnimRamPage();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	TWeakObjectPtr<class ADK_RamPage> RamPageOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = RamPage)
	uint8 bIsHoldingWall : 1;

};
