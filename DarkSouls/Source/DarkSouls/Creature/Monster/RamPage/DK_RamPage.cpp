// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/RamPage/DK_RamPage.h"
#include "PlayMontageCallbackProxy.h"

#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Attack/DK_AttackComponent.h"
#include "AOE/DK_AOE_Base.h"
#include "Interface/DK_DamageableInterface.h"

ADK_RamPage::ADK_RamPage()
{

}

void ADK_RamPage::BeginPlay()
{
	Super::BeginPlay();


}

bool ADK_RamPage::Skill_Combo0()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(0);

	// Section���� ���ε� �ϴ� �۾�
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindFunction_Skill_Combo0);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindFunction_Skill_Combo0()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::End_Skill_Combo0);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::End_Skill_Combo0);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginNotify_Skill_Combo0);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndNotify_Skill_Combo0);
}

void ADK_RamPage::BeginNotify_Skill_Combo0(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}
	else if (NotifyName == FName("AOESlash"))
	{
		FVector SlashLocation = GetMesh()->GetSocketLocation(FName("Combo1_Slash"));
		AttackComponent->AOEDamage(SlashLocation, 200.f, ComboComponent->GetCurrentAttackInfos());
	}
}

void ADK_RamPage::EndNotify_Skill_Combo0(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		EndAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		EndColRange_Notify();
	}
}

void ADK_RamPage::End_Skill_Combo0(FName NotifyName)
{
	InterruptedAttack_Notify();
}








bool ADK_RamPage::Skill_Combo1()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(1);

	// Section���� ���ε� �ϴ� �۾�
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindFunction_Skill_Combo1);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindFunction_Skill_Combo1()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::End_Skill_Combo1);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::End_Skill_Combo1);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginNotify_Skill_Combo1);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndNotify_Skill_Combo1);
}

void ADK_RamPage::BeginNotify_Skill_Combo1(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}
	else if (NotifyName == FName("AOESlash_1"))
	{
		FVector SlashLocation = GetMesh()->GetSocketLocation(FName("Combo1_Smash"));
		AttackComponent->AOEDamage(SlashLocation, 500.f, ComboComponent->GetCurrentAttackInfos());
	}
	else if (NotifyName == FName("AOESlash_2"))
	{
		FVector SlashLocation = GetMesh()->GetSocketLocation(FName("Combo1_Smash"));
		AttackComponent->AOEDamage(SlashLocation, 1000.f, ComboComponent->GetCurrentAttackInfos());
	}
}

void ADK_RamPage::EndNotify_Skill_Combo1(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		EndAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		EndColRange_Notify();
	}
}

void ADK_RamPage::End_Skill_Combo1(FName NotifyName)
{
	InterruptedAttack_Notify();
}
