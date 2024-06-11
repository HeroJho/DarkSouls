// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_HUDWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"

#include "UI/DK_BossSmoothBarWidget.h"
#include "UI/DK_SmoothBarWidget.h"


UDK_HUDWidget::UDK_HUDWidget()
{

}

void UDK_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBarWidget = Cast<UDK_SmoothBarWidget>(GetWidgetFromName(TEXT("WBHpBar")));
	SPBarWidget = Cast<UDK_SmoothBarWidget>(GetWidgetFromName(TEXT("WBSpBar")));
	TPBarWidget = Cast<UDK_SmoothBarWidget>(GetWidgetFromName(TEXT("WBTpBar")));
	
	BossHpList = Cast<UVerticalBox>(GetWidgetFromName(TEXT("BossHpList")));

	UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(BossHpList->Slot);
	BossHpListInitPos = Canvas->GetPosition();
	
}

void UDK_HUDWidget::UpdateHpBar(uint32 IN_Cur, uint32 IN_Max)
{
	HPBarWidget->UpdateBar(IN_Cur, IN_Max);
}

void UDK_HUDWidget::UpdateSpBar(uint32 IN_Cur, uint32 IN_Max)
{
	SPBarWidget->UpdateBar(IN_Cur, IN_Max);
}

void UDK_HUDWidget::UpdateTpBar(uint32 IN_Cur, uint32 IN_Max)
{
	TPBarWidget->UpdateBar(IN_Cur, IN_Max);
}





void UDK_HUDWidget::UpdateMaxHpBar(uint32 IN_Cur, uint32 IN_Max)
{
	UCanvasPanelSlot* Panel = Cast<UCanvasPanelSlot>(HPBarWidget->Slot);
	Panel->SetSize(FVector2D(IN_Max, 25));

	UpdateHpBar(IN_Cur, IN_Max);
}

void UDK_HUDWidget::UpdateMaxSpBar(uint32 IN_Cur, uint32 IN_Max)
{
	UCanvasPanelSlot* Panel = Cast<UCanvasPanelSlot>(SPBarWidget->Slot);
	Panel->SetSize(FVector2D(IN_Max, 25));

	UpdateSpBar(IN_Cur, IN_Max);
}

bool UDK_HUDWidget::MakeBossHpBar(UDK_SmoothBarWidget** OUT_BossHpBar, FText BossName)
{
	if (BossHpList->GetChildrenCount() > MaxBossHpBarCnt)
		return false;

	UDK_BossSmoothBarWidget* BossHpBarWidget = Cast<UDK_BossSmoothBarWidget>(CreateWidget(this, BossHpBarWidgetClass));
	BossHpList->AddChildToVerticalBox(BossHpBarWidget);
	BossHpBarWidget->SetBossName(BossName);


	// 3개 이상이면 변화 x
	if (BossHpList->GetChildrenCount() <= 3)
	{
		UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(BossHpList->Slot);
		FVector2D CurPos = Canvas->GetPosition();
		CurPos.Y -= 40;
		Canvas->SetPosition(CurPos);
	}
	

	*OUT_BossHpBar = BossHpBarWidget;

	return true;
}

void UDK_HUDWidget::RemoveBossHpBar(UDK_SmoothBarWidget* BossHpBar)
{
	BossHpList->RemoveChild(BossHpBar);

	UCanvasPanelSlot* Canvas = Cast<UCanvasPanelSlot>(BossHpList->Slot);

	if (BossHpList->GetChildrenCount() == 0)
	{
		Canvas->SetPosition(BossHpListInitPos);
	}
	else if (BossHpList->GetChildrenCount() < 3)
	{
		FVector2D CurPos = Canvas->GetPosition();
		CurPos.Y += 40;
		Canvas->SetPosition(CurPos);
	}
}
