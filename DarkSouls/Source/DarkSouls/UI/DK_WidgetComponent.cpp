// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DK_WidgetComponent.h"

#include "UI/DK_UserWidget.h"



void UDK_WidgetComponent::InitWidget()
{
	Super::InitWidget();

	UDK_UserWidget* ABUserWidget = Cast<UDK_UserWidget>(GetWidget());
	if (ABUserWidget)
	{
		ABUserWidget->SetOwningActor(GetOwner());
	}
}
