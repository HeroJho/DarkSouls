// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/S_DamageInfo.h"

FS_DamageInfo::FS_DamageInfo()
{
}

FS_DamageInfo::FS_DamageInfo(int32 InAmount, EDamageType InDamageType, EDamageResponse InDamageResponse, 
	uint8 InbShouldForceInterrupt, uint8 InbShouldDamageInvincible, uint8 InbCanBeBlocked, uint8 InbCanBeParried)
{
	Amount = InAmount;
	DamageType = InDamageType;
	DamageResponse = InDamageResponse;
	bShouldForceInterrupt = InbShouldForceInterrupt;
	bShouldDamageInvincible = InbShouldDamageInvincible;
	bCanBeBlocked = InbCanBeBlocked;
	bCanBeParried = InbCanBeParried;

}
