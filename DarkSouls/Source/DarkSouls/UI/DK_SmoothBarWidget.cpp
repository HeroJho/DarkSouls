// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_SmoothBarWidget.h"
#include "Components/ProgressBar.h"

#include "Tool/Define.h"


UDK_SmoothBarWidget::UDK_SmoothBarWidget()
{
}

void UDK_SmoothBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 블루프린트에서 작업한 오브젝트들이 생성되는 시점

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PbBar")));
	ensure(HpProgressBar);


}

void UDK_SmoothBarWidget::UpdateHpBar(uint32 IN_CurHP, uint32 IN_MaxHP)
{
	CurHP = (float)IN_CurHP;
	MaxHP = (float)IN_MaxHP;

	GetWorld()->GetTimerManager().ClearTimer(HpBarTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(HpBarTimerHandle, this, &UDK_SmoothBarWidget::TickHpBar, 1/60.f, true);

}

void UDK_SmoothBarWidget::TickHpBar()
{
	const float StatAdjSpeed = 5.f;

	float DisHP = CurHP - HPAcc;

	// TODO : DeltaTime이 엄청 커지면 어떻게 되지?
	if (DisHP > 0.f)
	{
		HPAcc = FMath::Clamp(HPAcc + DisHP * GetWorld()->DeltaTimeSeconds * StatAdjSpeed, 0.f, CurHP);
	}
	else
	{
		HPAcc = FMath::Clamp(HPAcc + DisHP * GetWorld()->DeltaTimeSeconds * StatAdjSpeed, CurHP, MaxHP);
	}

	
	if (FMath::IsNearlyEqual(HPAcc, CurHP, 1.f))
	{
		HPAcc = CurHP;
		GetWorld()->GetTimerManager().ClearTimer(HpBarTimerHandle);
	}

	HpProgressBar->SetPercent(HPAcc/MaxHP);

	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green, FString::Printf(TEXT("CurHP: %f / HPAcc: %f"), CurHP, HPAcc));
}
