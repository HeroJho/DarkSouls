// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AdaptiveGenNum/DK_AdaptiveGenNumComponent.h"

// Sets default values for this component's properties
UDK_AdaptiveGenNumComponent::UDK_AdaptiveGenNumComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_AdaptiveGenNumComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




void UDK_AdaptiveGenNumComponent::CreateNewTable(int TableIndex, TArray<FAdaptiveNodeInfo>& Infos)
{
	if (IsExistTable(TableIndex))
		return;

	FAdaptiveTableInfo TempTable; 

	int TotalToken = 0;
	for (int32 i = 0; i < Infos.Num(); ++i)
	{
		Infos[i].TokenNum = Infos[i].DefaultTokenNum;
		TotalToken += Infos[i].DefaultTokenNum;
	}

	TempTable.TableIndex = TableIndex;
	TempTable.TotalTokenNum = TotalToken;
	TempTable.Nodes = Infos;

	AdaptiveTable.Add(TableIndex, TempTable);

}

int UDK_AdaptiveGenNumComponent::GenAdaptiveNum(int TableIndex)
{
	int SelectedIndex = -1;

	// 테이블 확인
	FAdaptiveTableInfo* Table = AdaptiveTable.Find(TableIndex);
	if (Table == nullptr)
		return -1;

	int GendNum = FMath::RandRange(0, Table->TotalTokenNum);

	// 노드 선택
	int AccNum = 0;
	for (int32 i = 0; i < Table->Nodes.Num(); ++i)
	{
		AccNum += Table->Nodes[i].TokenNum;

		if (AccNum > GendNum)
		{
			SelectedIndex = i;
			break;
		}
	}

	if (SelectedIndex == -1)
		return -1;




	// 적응형 가중치 적용
	// 나눠줄 토큰 계산
	int DisToken = Table->Nodes[SelectedIndex].TokenNum - Table->Nodes[SelectedIndex].DownDis;
	if (DisToken < 0)
	{
		Table->Nodes[SelectedIndex].TokenNum = 0;
		DisToken = Table->Nodes[SelectedIndex].DownDis + DisToken;
	}
	else
	{
		Table->Nodes[SelectedIndex].TokenNum = DisToken;
		DisToken = Table->Nodes[SelectedIndex].DownDis;
	}

	// 토큰 분배
	int NodeSize = Table->Nodes.Num() - 1;

	int DivToken = DisToken / NodeSize;
	int RemainToken = DisToken % NodeSize;

	int PivReTokenIndex = FMath::RandRange(0, NodeSize);
	for (int32 i = 0; i <= NodeSize; ++i)
	{
		if (i == SelectedIndex)
			continue;

		Table->Nodes[i].TokenNum += DivToken;

		// 나머지 토큰 랜덤하게 분배
		if (RemainToken > 0)
		{
			int ReTokenIndex = (PivReTokenIndex + i) % Table->Nodes.Num();
			Table->Nodes[ReTokenIndex].TokenNum++;
			--RemainToken;
		}

	}


	DebugToken(TableIndex);


	return SelectedIndex;
}

void UDK_AdaptiveGenNumComponent::ResetTable(int TableIndex)
{
	FAdaptiveTableInfo* Table = AdaptiveTable.Find(TableIndex);
	if (Table == nullptr)
		return;

	for (int32 i = 0; i < Table->Nodes.Num(); ++i)
	{
		Table->Nodes[i].TokenNum = Table->Nodes[i].DefaultTokenNum;
	}

}





bool UDK_AdaptiveGenNumComponent::IsExistTable(int TableIndex)
{
	FAdaptiveTableInfo* Table = AdaptiveTable.Find(TableIndex);
	if (Table == nullptr)
		return false;

	return true;
}

void UDK_AdaptiveGenNumComponent::DebugToken(int TableIndex)
{
	FAdaptiveTableInfo* Table = AdaptiveTable.Find(TableIndex);
	if (Table == nullptr)
		return;

	int Total = 0;
	for (int32 i = 0; i < Table->Nodes.Num(); ++i)
	{
		Total += Table->Nodes[i].TokenNum;

		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, 
			FString::Printf(TEXT("%d / Token: %d"), i, Table->Nodes[i].TokenNum));
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("%d / %d"), Total, Table->TotalTokenNum));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan,
		FString::Printf(TEXT("==================")));

}