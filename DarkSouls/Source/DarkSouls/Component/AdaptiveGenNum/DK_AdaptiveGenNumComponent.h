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
	UDK_AdaptiveGenNumComponent();

protected:
	virtual void BeginPlay() override;


public:
	// 테이블 생성
	void CreateNewTable(int TableIndex, TArray<FAdaptiveNodeInfo>& Infos);
	// TableIndex기반으로 랜덤 숫자 생성
	int GenAdaptiveNum(int TableIndex);
	// TableIndex에서 SelectedNum를 선택했을 때 가중치 계산
	void CalculAdaptiveToken(int TableIndex, int SelectedNum);
	
	// 테이블 초기화
	void ResetTable(int TableIndex);

	bool IsExistTable(int TableIndex);

protected:	
	void DebugToken(int TableIndex);

protected:
	// TableIndex, Nodes 관리
	TMap<int, FAdaptiveTableInfo> AdaptiveTable;

};
