// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_BossSmoothBarWidget.h"
#include "Components/ProgressBar.h"


#include "Components/TextBlock.h"


UDK_BossSmoothBarWidget::UDK_BossSmoothBarWidget()
{
}



void UDK_BossSmoothBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UDK_SmoothBarWidget* HpBar = Cast<UDK_SmoothBarWidget>(GetWidgetFromName(TEXT("WBHpBar")));
	ProgressBar = HpBar->GetProgressBar();
	BossName = Cast<UTextBlock>(GetWidgetFromName(TEXT("Name")));

}

void UDK_BossSmoothBarWidget::UpdateBar(uint32 IN_Cur, uint32 IN_Max)
{
	Super::UpdateBar(IN_Cur, IN_Max);

}

void UDK_BossSmoothBarWidget::SetBossName(FText Name)
{
	BossName->SetText(Name);
}
