// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/RamPage/DK_RamPage.h"
#include "PlayMontageCallbackProxy.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Attack/DK_AttackComponent.h"
#include "Component/Stat/DK_StatComponent.h"
#include "AOE/DK_AOE_Base.h"
#include "Interface/DK_DamageableInterface.h"
#include "Creature/Monster/DK_AIControllerBase.h"

ADK_RamPage::ADK_RamPage()
{

}

void ADK_RamPage::BeginPlay()
{
	Super::BeginPlay();

	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_Skill_Combo0);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboSkill_Combo0);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboSkill_Combo0);

	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_GroundSmash);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboGroundSmash);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboGroundSmash);

	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_JumpAttack);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboJumpAttack);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboJumpAttack);

}





bool ADK_RamPage::Skill_Combo0()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(0);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_Skill_Combo0()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo0);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo0);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_Skill_Combo0);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_Skill_Combo0);
}

void ADK_RamPage::BeginSectionNotify_Skill_Combo0(FName NotifyName)
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

void ADK_RamPage::EndSectionNotify_Skill_Combo0(FName NotifyName)
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

void ADK_RamPage::EndSection_Skill_Combo0(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboSkill_Combo0()
{

}

void ADK_RamPage::End_ComboSkill_Combo0()
{

}








bool ADK_RamPage::GroundSmash()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(1);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_GroundSmash()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_GroundSmash);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_GroundSmash);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_GroundSmash);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_GroundSmash);
}

void ADK_RamPage::BeginSectionNotify_GroundSmash(FName NotifyName)
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

void ADK_RamPage::EndSectionNotify_GroundSmash(FName NotifyName)
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

void ADK_RamPage::EndSection_GroundSmash(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboGroundSmash()
{
}

void ADK_RamPage::End_ComboGroundSmash()
{
}













bool ADK_RamPage::JumpAttack()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(2);
	
	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_JumpAttack()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_JumpAttack);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_JumpAttack);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_JumpAttack);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_JumpAttack);
}

void ADK_RamPage::BeginSectionNotify_JumpAttack(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}
	else if (NotifyName == FName("Jump"))
	{
		bIsInJumpAttackPath = true;
		StatComponent->SetIsInvincible(true);

		AActor* Target = AIControllerBase->GetAttackTarget();

		// EndJump ¹ÙÀÎµù
		AttackComponent->Delegate_EndJump.Clear();
		AttackComponent->Delegate_EndJump.AddUObject(this, &ADK_RamPage::EndPathJumpAttack);

		// TODO
		FS_JumpAttackInfo JumpAttackInfo(JumpAttackCurve, 1.f, 1.f, -1.f, 400.f, 1600.f, 0.1f, 0.5f, true, 400.f, true);
		AttackComponent->JumpToAttackTarget(Target, JumpAttackInfo);

	}
	else if (NotifyName == FName("JumpLoop"))
	{
		UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
		if (bIsInJumpAttackPath)
		{
			GetMesh()->GetAnimInstance()->Montage_Pause();
		}

	}

}

void ADK_RamPage::EndSectionNotify_JumpAttack(FName NotifyName)
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

void ADK_RamPage::EndSection_JumpAttack(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::EndPathJumpAttack()
{
	FVector SlashLocation = GetMesh()->GetSocketLocation(FName("Combo1_Smash"));
	AttackComponent->AOEDamage(SlashLocation, 1500.f, ComboComponent->GetCurrentAttackInfos(), false);

	FTransform EffectMatrix;
	EffectMatrix.SetLocation(SlashLocation);
	EffectMatrix.SetScale3D({1.5f,1.5f,1.5f});
	EffectMatrix.SetRotation(FRotator(0.f, 90.f, 90.f).Quaternion());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpAttackParticle, EffectMatrix);

	bIsInJumpAttackPath = false;
	StatComponent->SetIsInvincible(false);
}



void ADK_RamPage::Interrupted_ComboJumpAttack()
{
	bIsInJumpAttackPath = false;
	StatComponent->SetIsInvincible(false);
}

void ADK_RamPage::End_ComboJumpAttack()
{
	bIsInJumpAttackPath = false;
	StatComponent->SetIsInvincible(false);
}
