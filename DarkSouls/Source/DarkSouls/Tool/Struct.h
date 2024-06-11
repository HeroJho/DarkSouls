#pragma once



struct FAttackDamagedInfo
{
	float Damage;
	int32 GPValue;

	bool bIsDown;

	bool bSetStunTimeToHitAnim;
	float StunTime;

	float HitPushPowar;
	float KnockDownPushPowar;
	float BlockPushPowar;

};