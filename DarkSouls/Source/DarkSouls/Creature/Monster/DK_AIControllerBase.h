// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DK_AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API ADK_AIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	ADK_AIControllerBase();


protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn);

protected:
	void RunAI();
	void StopAI();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UBehaviorTree> BTAsset;
	


};
