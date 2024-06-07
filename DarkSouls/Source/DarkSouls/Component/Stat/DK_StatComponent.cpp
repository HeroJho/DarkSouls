// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_StatComponent.h"

#include "Tool/Define.h"

// Sets default values for this component's properties
UDK_StatComponent::UDK_StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_StatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDK_StatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}





void UDK_StatComponent::ResetStat()
{
	IncreaseHP(MaxHP);

}


void UDK_StatComponent::AddChangeHPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ChangeHP.AddUFunction(Object, FuncName);
}

void UDK_StatComponent::AddZeroHPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_ZeroHP.AddUFunction(Object, FuncName);
}







void UDK_StatComponent::IncreaseHP(int Value)
{
	CurHP = FMath::Clamp(CurHP + Value, 0, MaxHP);

	Delegate_ChangeHP.Broadcast(CurHP, MaxHP);
}

void UDK_StatComponent::DecreaseHP(int Value)
{
	CurHP = FMath::Clamp(CurHP - Value, 0, MaxHP);
	

	if (CurHP == 0)
	{
		// ªÁ∏¡ µ®∏Æ∞‘¿Ã∆Æ

		Delegate_ZeroHP.Broadcast();
	}


	Delegate_ChangeHP.Broadcast(CurHP, MaxHP);
}


