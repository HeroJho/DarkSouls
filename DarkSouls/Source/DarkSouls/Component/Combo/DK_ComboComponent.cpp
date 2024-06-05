// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Combo/DK_ComboComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"

#include "Creature/DK_Creature.h"
#include "Component/Combo/DK_ComboActionData.h"

// Sets default values for this component's properties
UDK_ComboComponent::UDK_ComboComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	
}


// Called when the game starts
void UDK_ComboComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	Owner = Cast<ADK_Creature>(GetOwner());
	
	ChangeComboActionData(0);
}


void UDK_ComboComponent::ProcessComboCommand()
{
	// 예약 상태라면 기존 재생하던 섹션만 재생한다
	if (ReserveComboActionDataIndex != -1)
	{
		return;
	}

	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}
	else
	{
		bHasNextComboCommand = true;
	}

}

void UDK_ComboComponent::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;
	bHasNextComboCommand = false;
	bIsCallEnd = true;

	// Movement Setting
	// Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	// Animation Setting
	// AnimInstance->Montage_Play(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage, AttackSpeedRate);

	UnBindEndDelegate();

	Owner->PlayAnimMontage(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage);

	BindEndDelegate();

}

void UDK_ComboComponent::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{

	// 스킵해야하는 상황 (마지막 콤보가 아니고, 키입력을 받았을 때)
	uint8 MaxCount = ComboActionDatas[CurComboActionDataIndex]->MaxComboCount;
	if (CurrentCombo != MaxCount && !bIsCallEnd)
	{
		return;
	}

	ensure(CurrentCombo != 0);

	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsCallEnd = true;

	// Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	if (ReserveComboActionDataIndex != -1)
	{
		CurComboActionDataIndex = ReserveComboActionDataIndex;
		ReserveComboActionDataIndex = -1;
	}

	
}

void UDK_ComboComponent::BindEndDelegate()
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UDK_ComboComponent::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage);
}

void UDK_ComboComponent::UnBindEndDelegate()
{
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance(); 
	FOnMontageEnded* PreEndDelegate = AnimInstance->Montage_GetEndedDelegate();
	if (PreEndDelegate)
		PreEndDelegate->Unbind();

}

void UDK_ComboComponent::ComboCheck()
{
	if (bHasNextComboCommand)
	{
		UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *CurData->MontageSectionNamePrefix, CurrentCombo);


		// *PlayAnimMontage를 하고 등록한 델리게이트가 해당 섹션에서 적용이 되는 뜻.
		UnBindEndDelegate();
		
		// * JumpToSection은 Section 사이에 블랜딩이 안됨.
		// AnimInstance->Montage_JumpToSection(NextSection, CurData->ComboActionMontage);
		Owner->PlayAnimMontage(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage, 1.f, NextSection);

		BindEndDelegate();

		bHasNextComboCommand = false;
		bIsCallEnd = false;
	}
	else
	{
		// 콤보가 넘어가는거면 이번 애니메이션의 End는 호출하지 않는다
		bIsCallEnd = true;
	}

}

void UDK_ComboComponent::ChangeComboActionData(uint8 DataIndex)
{
	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsCallEnd = true;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		// 몽타주가 실행중인데, 콤보를 바꾼다? -> 기존꺼 끝내고 바꾼다
		if (AnimInstance->IsAnyMontagePlaying())
		{
			ReserveComboActionDataIndex = DataIndex;
		}
		else
		{
			CurComboActionDataIndex = DataIndex;
			ReserveComboActionDataIndex = -1;
		}
	}

}

const TArray<FString>& UDK_ComboComponent::GetCurrentAttackCollisionInfos()
{
	return ComboActionDatas[CurComboActionDataIndex]->AttackColInfos[CurrentCombo].AttackCollisions;
}

void UDK_ComboComponent::GetCurrentAttackInfos(float& OUT_Damage, bool& OUT_bIsDown, bool& OUT_bSetStunTimeToHitAnim, float& OUT_StunTime)
{
	const FAttackInfo& AttackInfo = ComboActionDatas[CurComboActionDataIndex]->AttackColInfos[CurrentCombo];

	OUT_Damage = AttackInfo.Damage;
	OUT_bIsDown = AttackInfo.bIsDown;
	OUT_bSetStunTimeToHitAnim = AttackInfo.bSetStunTimeToHitAnim;
	OUT_StunTime = AttackInfo.StunTime;
}

void UDK_ComboComponent::ResetComboInfo()
{
	// 이 인덱스에 따라 콤보가 실행됨
	CurrentCombo = 0;
	bHasNextComboCommand = false;
	bIsCallEnd = false; // EndDelegate 호출 여부

	ReserveComboActionDataIndex = -1;
}
