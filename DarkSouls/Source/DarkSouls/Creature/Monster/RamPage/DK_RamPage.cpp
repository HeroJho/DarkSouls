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


}

bool ADK_RamPage::Skill_Combo0()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(0);

	// Section마다 바인딩 하는 작업
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








bool ADK_RamPage::GroundSmash()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(1);

	// Section마다 바인딩 하는 작업
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindFunction_GroundSmash);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindFunction_GroundSmash()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::End_GroundSmash);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::End_GroundSmash);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginNotify_GroundSmash);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndNotify_GroundSmash);
}

void ADK_RamPage::BeginNotify_GroundSmash(FName NotifyName)
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

void ADK_RamPage::EndNotify_GroundSmash(FName NotifyName)
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

void ADK_RamPage::End_GroundSmash(FName NotifyName)
{
	InterruptedAttack_Notify();
}













bool ADK_RamPage::JumpAttack()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData(2);

	// Section마다 바인딩 하는 작업
	ComboComponent->OnSectionEndDelegate.Clear();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindFunction_JumpAttack);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindFunction_JumpAttack()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::End_JumpAttack);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::End_JumpAttack);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginNotify_JumpAttack);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndNotify_JumpAttack);
}

void ADK_RamPage::BeginNotify_JumpAttack(FName NotifyName)
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
		StatComponent->SetIsInvincible(true);

		bIsInJumpAttackPath = true;

		AActor* Target = AIControllerBase->GetAttackTarget();

		// EndJump 바인딩
		AttackComponent->Delegate_EndJump.Clear();
		AttackComponent->Delegate_EndJump.AddUObject(this, &ADK_RamPage::EndPathJumpAttack);

		// TODO
		FS_JumpAttackInfo JumpAttackInfo(JumpAttackCurve, 0.5f, 1.f, -1.f, 400.f, 1600.f, 0.1f, 0.5f, true, 400.f, false);
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

void ADK_RamPage::EndNotify_JumpAttack(FName NotifyName)
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

void ADK_RamPage::End_JumpAttack(FName NotifyName)
{
	InterruptedAttack_Notify();
	bIsInJumpAttackPath = false;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf("End_JumpAttack"));
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

	StatComponent->SetIsInvincible(false);
}
