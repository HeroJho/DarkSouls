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


// Called every frame
void UDK_ComboComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UDK_ComboComponent::ProcessComboCommand()
{
	// 예약 상태라면 넘긴다
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
		HasNextComboCommand = true;
	}

}

void UDK_ComboComponent::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;
	HasNextComboCommand = false;

	// Movement Setting
	// Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.f;
	
	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UDK_ComboComponent::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage);

}

void UDK_ComboComponent::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	Owner->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void UDK_ComboComponent::NotifyComboActionEnd()
{
	if (ReserveComboActionDataIndex != -1)
	{
		CurComboActionDataIndex = ReserveComboActionDataIndex;
		ReserveComboActionDataIndex = -1;
	}

}


void UDK_ComboComponent::ComboCheck()
{
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();
		UDK_ComboActionData* CurData = ComboActionDatas[CurComboActionDataIndex];

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *CurData->MontageSectionNamePrefix, CurrentCombo);

		Owner->PlayAnimMontage(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage, 1.f, NextSection);
		// * JumpToSection은 Section 사이에 블랜딩이 안됨.
		//AnimInstance->Montage_JumpToSection(NextSection, CurData->ComboActionMontage);
		//AnimInstance->Montage_Play(ComboActionDatas[CurComboActionDataIndex]->ComboActionMontage);

		HasNextComboCommand = false;
	}
	
}

void UDK_ComboComponent::ChangeComboActionData(uint8 DataIndex)
{
	CurrentCombo = 0;
	HasNextComboCommand = false;

	UAnimInstance* AnimInstance = Owner->GetMesh()->GetAnimInstance();

	if (AnimInstance)
	{
		// 몽타주가 실행중이다
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
