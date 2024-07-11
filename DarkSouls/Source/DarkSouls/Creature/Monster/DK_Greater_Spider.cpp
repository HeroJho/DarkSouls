// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/DK_Greater_Spider.h"

#include "Component/Combo/DK_ComboComponent.h"
#include "PlayMontageCallbackProxy.h"
#include "AOE/DK_AOE_Base.h"
#include "Interface/DK_DamageableInterface.h"

ADK_Greater_Spider::ADK_Greater_Spider()
{

}

void ADK_Greater_Spider::BeginPlay()
{
	Super::BeginPlay();


}


void ADK_Greater_Spider::Skill_Combo0()
{
	if (!CanAttack())
		return;

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(0);

	// Section마다 바인딩 하는 작업
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_Greater_Spider::BindFunction_Skill_Combo0);

	ComboComponent->ProcessComboCommand(true);
	
}

void ADK_Greater_Spider::BindFunction_Skill_Combo0()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_Greater_Spider::End_Skill_Combo0);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_Greater_Spider::End_Skill_Combo0);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_Greater_Spider::BeginNotify_Skill_Combo0);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_Greater_Spider::EndNotify_Skill_Combo0);

}

void ADK_Greater_Spider::BeginNotify_Skill_Combo0(FName NotifyName)
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
		const FTransform SpawnTransform(GetActorLocation());

		ADK_AOE_Base* SlashAOE = GetWorld()->SpawnActorDeferred<ADK_AOE_Base>(ADK_AOE_Base::StaticClass(), SpawnTransform, this, this);
		SlashAOE->InitOption(300.f, true, true, true);

		// TODO : Edit to Lamda
		SlashAOE->OnAOEOverlapActorDelegate.AddUObject(this, &ADK_Greater_Spider::AttackAOESmash);
				
		SlashAOE->FinishSpawning(SpawnTransform);
	}

}

void ADK_Greater_Spider::EndNotify_Skill_Combo0(FName NotifyName)
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

void ADK_Greater_Spider::End_Skill_Combo0(FName NotifyName)
{
	InterruptedAttack_Notify();

}

void ADK_Greater_Spider::AttackAOESmash(AActor* HitActor)
{
	IDK_DamageableInterface* HitActorDamageable = Cast<IDK_DamageableInterface>(HitActor);
	if (HitActorDamageable)
	{
		FS_DamageInfo DamageInfo = ComboComponent->GetCurrentAttackInfos();
		HitActorDamageable->TakeDamage(DamageInfo, this);
	}

}
