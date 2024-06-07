// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_HUDWidget.h"

#include "UI/DK_HPBarWidget.h"



UDK_HUDWidget::UDK_HUDWidget()
{

}

void UDK_HUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HPBarWidget = Cast<UDK_HPBarWidget>(GetWidgetFromName(TEXT("WBHpBar")));
	SPBarWidget = Cast<UDK_HPBarWidget>(GetWidgetFromName(TEXT("WBSpBar")));
	
}
