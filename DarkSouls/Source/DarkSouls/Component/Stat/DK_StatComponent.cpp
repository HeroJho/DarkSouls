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
	Delegate_ChangeHP.Broadcast(CurHP, MaxHP);
}


FDelegateHandle UDK_StatComponent::AddChangeHPDelegateFunc(UObject* Object, FName FuncName)
{
	return Delegate_ChangeHP.AddUFunction(Object, FuncName);
}

void UDK_StatComponent::RemoveChangeHPDelegateFunc(FDelegateHandle Handle)
{
	Delegate_ChangeHP.Remove(Handle);
}

void UDK_StatComponent::AddZeroHPDelegateFunc(UObject* Object, FName FuncName)
{
	Delegate_OnDeath.AddUFunction(Object, FuncName);
}







void UDK_StatComponent::IncreaseHP(int32 Value)
{
	CurHP = FMath::Clamp(CurHP + Value, 0, MaxHP);

	Delegate_ChangeHP.Broadcast(CurHP, MaxHP);
}


bool UDK_StatComponent::TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser)
{
	// ������� ���� �� �ִ� ���³�
	if (!bIsDead && (!bIsInvincible || DamageInfo.bShouldDamageInvincible))
	{
		// ��� ���³�
		if (bIsBlocking && DamageInfo.bCanBeBlocked)
		{
			// Block
			Delegate_OnBlock.Broadcast(DamageInfo.bCanBeParried, DamageCauser);
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
				Delegate_OnDeath.Broadcast();
			}
			else
			{
				// ���ͷ�Ʈ ����
				if (bIsInterruptible || DamageInfo.bShouldForceInterrupt)
				{
					Delegate_OnDamageResponse.Broadcast(DamageInfo.DamageResponse, DamageCauser);
				}
			}

			Delegate_ChangeHP.Broadcast(CurHP, MaxHP);

			return true;
		}

	}
	else
	{
		// No Damage
		return false;
	}

}


