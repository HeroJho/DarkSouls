// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_AdaptiveGenNumComponent.generated.h"

USTRUCT(BlueprintType)
struct FAdaptiveNodeInfo
{
	GENERATED_USTRUCT_BODY()


	UPROPERTY(EditAnywhere)
	int DefaultTokenNum = 0;
	UPROPERTY(EditAnywhere)
	int DownDis = 0;

	int TokenNum = 0;
};

USTRUCT(BlueprintType)
struct FAdaptiveTableInfo
{
	GENERATED_USTRUCT_BODY()


	int TableIndex = -1;
	int TotalTokenNum = 0;
	TArray<FAdaptiveNodeInfo> Nodes;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_AdaptiveGenNumComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_AdaptiveGenNumComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


public:
	void CreateNewTable(int TableIndex, TArray<FAdaptiveNodeInfo>& Infos);
	int GenAdaptiveNum(int TableIndex);
	void CalculAdaptiveToken(int TableIndex, int SelectedNum);

	void ResetTable(int TableIndex);

	bool IsExistTable(int TableIndex);

protected:	
	void DebugToken(int TableIndex);

protected:
	TMap<int, FAdaptiveTableInfo> AdaptiveTable;

};
