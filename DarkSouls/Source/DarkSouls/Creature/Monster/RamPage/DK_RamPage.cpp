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

void ADK_RamPage::Skill_Combo0()
{
	if (!CanAttack())
		return;

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(0);

	// Section마다 바인딩 하는 작업
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindFunction_Skill_Combo0);

	ComboComponent->ProcessComboCommand(true);

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
