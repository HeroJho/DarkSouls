// Fill out your copyright notice in the Description page of Project Settings.


#include "Struct/S_DamageInfo.h"


FS_JumpAttackInfo::FS_JumpAttackInfo()
{
}

FS_JumpAttackInfo::FS_JumpAttackInfo(UCurveFloat* InCurve, float InJumpSpeed, float InPredictTime,
	float InArc, float InMinDisRange, float InMaxDisRange, float InMinArc, float InMaxArc,
	bool InbIsFrontTarget, float InFrontDis, float InEndAnimPlayRatio, bool InbRenderDebug)
	: Curve(InCurve), JumpSpeed(InJumpSpeed),
	PredictTime(InPredictTime), Arc(InArc), MinDisRange(InMinDisRange), MaxDisRange(InMaxDisRange), MinArc(InMinArc), MaxArc(InMaxArc),
	bIsFrontTarget(InbIsFrontTarget), FrontDis(InFrontDis), EndAnimPlayRatio(InEndAnimPlayRatio), bRenderDebug(InbRenderDebug)
{

}


FS_DamageInfo::FS_DamageInfo()
{
}

FS_DamageInfo::FS_DamageInfo(int32 InAmount, EDamageType InDamageType, EDamageResponse InDamageResponse,
	uint8 InbShouldForceInterrupt, uint8 InbShouldDamageInvincible, uint8 InbCanBeBlocked, uint8 InbCanBeParried)
	: Amount(InAmount), DamageType(InDamageType), DamageResponse(InDamageResponse), 
	bShouldForceInterrupt(InbShouldForceInterrupt), bShouldDamageInvincible(InbShouldDamageInvincible), 
	bCanBeBlocked(InbCanBeBlocked), bCanBeParried(InbCanBeParried)
{
}
