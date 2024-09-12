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
	//    StanPos ���� MaxDis ������ Channel Ʈ���̽��� �浹�� ������Ʈ���� StanVec ���͸� �������� MaxAngle ���� �ִ� ������Ʈ�� ��ȯ 
	UFUNCTION(BlueprintCallable)
	const TArray<AActor*> GetObjectInAngleByChannel(ECollisionChannel Channel, FVector StanPos, float MaxDis, FVector StanVec, float MaxAngle, bool bDebug);
	UFUNCTION(BlueprintCallable)
	AActor* GetObjectInNearstAngleByChannel(ECollisionChannel Channel, FVector StanPos, float MaxDis, FVector StanVec, float MaxAngle, bool bDebug);

	UFUNCTION(BlueprintCallable)
	FVector CalculateFutureActorXYLocation(AActor* Target, float Time);

	UFUNCTION(BlueprintCallable)
	bool PredictProjectilePath(AActor* Me, AActor* Target, TArray<FVector>& OUT_Pos, float PredictTime, float Arc, bool bIsFrontTarget, float FrontDis,  bool bDebug = false);
};
