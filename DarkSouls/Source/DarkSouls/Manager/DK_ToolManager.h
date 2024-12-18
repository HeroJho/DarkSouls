// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "DK_ToolManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_ToolManager : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_ToolManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	//    StanPos 기준 MaxDis 반지름 Channel 트레이스에 충돌한 오브젝트에서 StanVec 벡터를 기준으로 MaxAngle 내에 있는 오브젝트를 반환 
	const TArray<AActor*> GetObjectInAngleByChannel(ECollisionChannel Channel, FVector MyPos, float MaxDis, FVector MyVec, float MaxAngle, bool bDebug);
	
	UFUNCTION(BlueprintCallable)
	AActor* GetObjectInNearstAngleByChannel(ECollisionChannel Channel, FVector MyPos, float MaxDis, FVector MyVec, float MaxAngle, bool bDebug);

	UFUNCTION(BlueprintCallable)
	FVector CalculateFutureActorXYLocation(AActor* Target, float Time);


	bool PredictProjectilePath(AActor* Me, AActor* Target, TArray<FVector>& OUT_Pos, float PredictTime, float Arc, float FrontDis,  bool bDebug = false);
	bool PredictProjectilePath(FVector From , FVector To, TArray<FVector>& OUT_Pos, float Arc, bool bDebug = false);
};
