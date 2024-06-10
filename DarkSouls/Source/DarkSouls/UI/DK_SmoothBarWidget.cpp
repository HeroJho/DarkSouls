// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_SmoothBarWidget.h"
#include "Components/ProgressBar.h"
//#include "Components/CanvasPanel.h"

#include "Tool/Define.h"


UDK_SmoothBarWidget::UDK_SmoothBarWidget()
{
}

void UDK_SmoothBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 블루프린트에서 작업한 오브젝트들이 생성되는 시점
	ProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbBar")));

}

void UDK_SmoothBarWidget::UpdateBar(uint32 IN_Cur, uint32 IN_Max)
{
	Cur = (float)IN_Cur;
	Max = (float)IN_Max;

	GetWorld()->GetTimerManager().ClearTimer(BarTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(BarTimerHandle, this, &UDK_SmoothBarWidget::TickBar, 1/60.f, true);

}

void UDK_SmoothBarWidget::TickBar()
{
	const float StatAdjSpeed = 5.f;

	float DisHP = Cur - Acc;


	if (DisHP > 0.f)
	{
		Acc = FMath::Clamp(Acc + DisHP * GetWorld()->DeltaTimeSeconds * StatAdjSpeed, 0.f, Cur);
	}
	else
	{
		Acc = FMath::Clamp(Acc + DisHP * GetWorld()->DeltaTimeSeconds * StatAdjSpeed, Cur, Max);
	}

	
	if (FMath::IsNearlyEqual(Acc, Cur, 1.f))
	{
		Acc = Cur;
		GetWorld()->GetTimerManager().ClearTimer(BarTimerHandle);
	}



	ProgressBar->SetPercent(Acc/Max);

}
