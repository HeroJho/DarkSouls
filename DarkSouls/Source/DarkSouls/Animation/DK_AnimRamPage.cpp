// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/DK_AnimRamPage.h"

 #include "Creature/Monster/RamPage/DK_RamPage.h"

UDK_AnimRamPage::UDK_AnimRamPage()
{
	bIsHoldingWall = false;
}

void UDK_AnimRamPage::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Owner)
	{
		RamPageOwner = Cast<ADK_RamPage>(Owner);
	}
}

void UDK_AnimRamPage::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if(RamPageOwner.IsValid())
	{
		bIsHoldingWall = RamPageOwner->GetIsHoldingWall();
	}

}
