// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Stat/DK_StatComponent.h"

#include "Tool/Define.h"
#include "StatData/DK_StatDataAsset.h"

// Sets default values for this component's properties
UDK_StatComponent::UDK_StatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
	bIsInterruptible = true;
	bIsInvincible = false;
	bIsBlocking = false;

}


// Called when the game starts
void UDK_StatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}




void UDK_StatComponent::ResetStat()
{
	MaxHP = StatData->MaxHP;
	IncreaseHP(MaxHP);
}

void UDK_StatComponent::BroadcastStat()
{
	OnChangeHPDelegate.Broadcast(CurHP, MaxHP);
}





void UDK_StatComponent::IncreaseHP(int32 Value)
{
	CurHP = FMath::Clamp(CurHP + Value, 0, MaxHP);

	OnChangeHPDelegate.Broadcast(CurHP, MaxHP);
}

bool UDK_StatComponent::CheckBlock(FVector AttackerPos)
{
	const float BlockAngle = 90.f;

	FVector MeLook = GetOwner()->GetActorForwardVector();
	FVector LookAtTargetVec = AttackerPos - GetOwner()->GetActorLocation();
	LookAtTargetVec.Normalize();
	MeLook.Normalize();

	float  DotResult = FVector::DotProduct(MeLook, LookAtTargetVec);
	float Angle = (FMath::Acos(DotResult) / UE_PI) * 180.f;

	if (FMath::Abs(Angle) > BlockAngle)
		return false;


	return true;
}


bool UDK_StatComponent::TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser)
{
	if (!IsValid(DamageCauser))
		return false;

	// ������� ���� �� �ִ� ���³�
	if (!bIsDead && (!bIsInvincible || DamageInfo.bShouldDamageInvincible))
	{
		// ��� ���³� && ��� ������ �����̳� && ��� ���̳� 
		if (bIsDodgSkip)
		{
			// Dodge
			OnDodgSkipDelegate.Broadcast();
			return false;
		}	
		else if (bIsBlocking && DamageInfo.bCanBeBlocked && CheckBlock(DamageCauser->GetActorLocation()))
		{
			// Block
			OnBlockDelegate.Broadcast(DamageInfo.bCanBeParried, DamageInfo.BlockKnockBackPowar, DamageCauser);
			return false;
		}
		else
		{
			// Do Damage
			int32 Value = DamageInfo.Amount;

			CurHP = FMath::Clamp(CurHP - Value, 0, MaxHP);

			// �׾��� �� �׾���
			if (CurHP <= 0)
			{
				CurHP = 0;
				bIsDead = true;
				OnDeathDelegate.Broadcast();
			}
			else
			{
				// ���ͷ�Ʈ ����
				if (bIsInterruptible || DamageInfo.bShouldForceInterrupt)
				{
					OnDamageResponseDelegate.Broadcast(DamageInfo.DamageResponse, DamageInfo.GPValue, DamageInfo.StunTime, DamageInfo.HitKnockBackPowar, DamageCauser);
				}
			}

			OnChangeHPDelegate.Broadcast(CurHP, MaxHP);

			return true;
		}

	}
	else
	{
		// No Damage
		return false;
	}

}


