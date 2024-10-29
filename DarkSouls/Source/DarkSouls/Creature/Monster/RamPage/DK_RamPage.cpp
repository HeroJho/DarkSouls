// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/RamPage/DK_RamPage.h"
#include "PlayMontageCallbackProxy.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Attack/DK_AttackComponent.h"
#include "Component/Stat/DK_StatComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Creature/Monster/DK_AIControllerBase.h"
#include "Creature/Monster/RamPage/E_RamPage_Attacks.h"
#include "AOE/DK_AOE_Base.h"
#include "Interface/DK_DamageableInterface.h"
#include "Projectile/DK_Projectile_Base.h"



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

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::Combo1);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_Skill_Combo0);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboSkill_Combo0);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboSkill_Combo0);

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
	InterruptedAttack_Notify();
	EndAttack_Notify();
}

void ADK_RamPage::End_ComboSkill_Combo0()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}






bool ADK_RamPage::Skill_Combo1()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::Combo2);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_Skill_Combo1);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboSkill_Combo1);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboSkill_Combo1);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_Skill_Combo1()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo1);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo1);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_Skill_Combo1);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_Skill_Combo1);
}

void ADK_RamPage::BeginSectionNotify_Skill_Combo1(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}

}

void ADK_RamPage::EndSectionNotify_Skill_Combo1(FName NotifyName)
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

void ADK_RamPage::EndSection_Skill_Combo1(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboSkill_Combo1()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}

void ADK_RamPage::End_ComboSkill_Combo1()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}






bool ADK_RamPage::Skill_Combo2()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::Combo3);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_Skill_Combo2);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboSkill_Combo2);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboSkill_Combo2);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_Skill_Combo2()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo2);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_Skill_Combo2);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_Skill_Combo2);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_Skill_Combo2);
}

void ADK_RamPage::BeginSectionNotify_Skill_Combo2(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}

}

void ADK_RamPage::EndSectionNotify_Skill_Combo2(FName NotifyName)
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

void ADK_RamPage::EndSection_Skill_Combo2(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboSkill_Combo2()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}

void ADK_RamPage::End_ComboSkill_Combo2()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}










bool ADK_RamPage::GroundSmash()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::GroundSmash);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_GroundSmash);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboGroundSmash);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboGroundSmash);

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
	InterruptedAttack_Notify();
	EndAttack_Notify();
}

void ADK_RamPage::End_ComboGroundSmash()
{
	InterruptedAttack_Notify();
	EndAttack_Notify();
}






//
//bool ADK_RamPage::ThrowWall()
//{
//	if (!CanAttack())
//	{
//		return false;
//	}
//
//	ResetInfoOnAttack();
//
//	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::ThrowWall);
//
//	ComboComponent->ClearDelegate();
//	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_ThrowWall);
//	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboThrowWall);
//	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboThrowWall);
//
//	ComboComponent->ProcessComboCommand(true);
//
//	bIsAttacking = true;
//
//	return true;
//}
//
//void ADK_RamPage::BindSectionFunction_ThrowWall()
//{
//	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
//	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_ThrowWall);
//	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_ThrowWall);
//	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_ThrowWall);
//	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_ThrowWall);
//}
//
//void ADK_RamPage::BeginSectionNotify_ThrowWall(FName NotifyName)
//{
//	if (NotifyName == FName("AttackRange"))
//	{
//		BeginAttackRange_Notify();
//	}
//	else if (NotifyName == FName("ColRange"))
//	{
//		BeginColRange_Notify();
//	}
//	else if (NotifyName == FName("FullWall"))
//	{
//		// 중간에 플레이어가 죽었다
//		AActor* Target = AIControllerBase->GetAttackTarget();
//
//		if (!IsValid(Target))
//		{
//			StopAnimMontage();
//			return;
//		}
//			
//
//		if (ThrowWallProjectile.IsValid())
//			ThrowWallProjectile->DestroyProjectile();
//
//		ThrowWallProjectile = GetWorld()->SpawnActorDeferred<ADK_Projectile_Base>(ThrowWallProjectileClass, FTransform::Identity, this);
//
//
//
//		ProjectileOption ProjOp(Target, 10.f, -1.f, 4500.f, 0.5f, false, false);
//		ThrowWallProjectile->Init(ProjOp);
//		ThrowWallProjectile->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("RockAttachPoint")));
//
//		// 람다 바인드 -> 따로 멤버 함수 바인딩하면 TempWall 부분에서 Nullptr이 된다
//		FS_DamageInfo DamageInfo = ComboComponent->GetCurrentAttackInfos();
//		AActor* TempOwner = GetOwner();
//		TWeakObjectPtr<ADK_Projectile_Base> TempWall = ThrowWallProjectile;
//		ThrowWallProjectile->OnProjectileImpact.BindLambda([DamageInfo, TempOwner, TempWall](AActor* HitActor, const FHitResult& Hit)
//			{
//				if (!IsValid(TempOwner))
//					return;
//
//				IDK_DamageableInterface* HitActorDamageable = Cast<IDK_DamageableInterface>(HitActor);
//				if (HitActorDamageable == nullptr)
//					return;
//
//				HitActorDamageable->TakeDamage(DamageInfo, TempOwner);
//
//				bool bIsHit = HitActorDamageable->TakeDamage(DamageInfo, TempOwner);
//				if (!bIsHit)
//					return;
//				
//				if (!TempWall.IsValid())
//					return;
//
//				TempWall->DestroyProjectile(Hit.Location);
//				
//			});
//
//
//		CollisionManagerComponent->IgnoreCol(ThrowWallProjectile.Get());
//
//
//
//		ThrowWallProjectile->FinishSpawning(FTransform::Identity);
//
//
//	}
//	else if (NotifyName == FName("Throw"))
//	{
//
//		if (ThrowWallProjectile.IsValid())
//		{
//			ThrowWallProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
//			ThrowWallProjectile->SimulatingProjectile();
//
//			ThrowWallProjectile = nullptr;
//		}
//
//	}
//
//}
//
//void ADK_RamPage::EndSectionNotify_ThrowWall(FName NotifyName)
//{
//	if (NotifyName == FName("AttackRange"))
//	{
//		EndAttackRange_Notify();
//	}
//	else if (NotifyName == FName("ColRange"))
//	{
//		EndColRange_Notify();
//	}
//}
//
//void ADK_RamPage::EndSection_ThrowWall(FName NotifyName)
//{
//	InterruptedAttack_Notify();
//}
//
//void ADK_RamPage::Interrupted_ComboThrowWall()
//{
//	InterruptedAttack_Notify();
//
//	if (ThrowWallProjectile.IsValid())
//	{
//		ThrowWallProjectile->DestroyProjectile();
//	}
//
//}
//
//void ADK_RamPage::End_ComboThrowWall()
//{
//	InterruptedAttack_Notify();
//
//	if (ThrowWallProjectile.IsValid())
//	{
//		ThrowWallProjectile->DestroyProjectile();
//	}
//
//}
//
//
//




bool ADK_RamPage::FullWall()
{
	if (!CanAttack())
	{
		return false;
	}

	// 돌을들고 있다면
	DestroyWall();



	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::FullWall);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_FullWall);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboFullWall);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboFullWall);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_FullWall()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_FullWall);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_FullWall);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_FullWall);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_FullWall);
}

void ADK_RamPage::BeginSectionNotify_FullWall(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}
	else if (NotifyName == FName("FullWall"))
	{
		// 중간에 플레이어가 죽었다
		AActor* Target = AIControllerBase->GetAttackTarget();

		if (!IsValid(Target))
		{
			StopAnimMontage();
			return;
		}

		CreateWall(Target);

	}



}

void ADK_RamPage::EndSectionNotify_FullWall(FName NotifyName)
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

void ADK_RamPage::EndSection_FullWall(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboFullWall()
{
	InterruptedAttack_Notify();

	EndAttack_Notify();
}

void ADK_RamPage::End_ComboFullWall()
{
	InterruptedAttack_Notify();

	EndAttack_Notify();
}




bool ADK_RamPage::GetIsHoldingWall()
{
	return ThrowWallProjectile.IsValid();
}

void ADK_RamPage::CreateWall(AActor* Target)
{
	if (ThrowWallProjectile.IsValid())
		ThrowWallProjectile->DestroyProjectile();

	ThrowWallProjectile = GetWorld()->SpawnActorDeferred<ADK_Projectile_Base>(ThrowWallProjectileClass, FTransform::Identity, this);



	ProjectileOption ProjOp(Target, 10.f, -1.f, 4500.f, 0.5f, false, false);
	ThrowWallProjectile->Init(ProjOp);
	ThrowWallProjectile->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName(TEXT("RockAttachPoint")));

	// 람다 바인드 -> 따로 멤버 함수 바인딩하면 TempWall 부분에서 Nullptr이 된다
	FS_DamageInfo DamageInfo = ComboComponent->GetCurrentAttackInfos();
	AActor* TempOwner = GetOwner();
	TWeakObjectPtr<ADK_Projectile_Base> TempWall = ThrowWallProjectile;
	ThrowWallProjectile->OnProjectileImpact.BindLambda([DamageInfo, TempOwner, TempWall](AActor* HitActor, const FHitResult& Hit)
		{
			if (!IsValid(TempOwner))
				return;

			IDK_DamageableInterface* HitActorDamageable = Cast<IDK_DamageableInterface>(HitActor);
			if (HitActorDamageable == nullptr)
				return;

			HitActorDamageable->TakeDamage(DamageInfo, TempOwner);

			bool bIsHit = HitActorDamageable->TakeDamage(DamageInfo, TempOwner);
			if (!bIsHit)
				return;

			if (!TempWall.IsValid())
				return;

			TempWall->DestroyProjectile(Hit.Location);

		});


	CollisionManagerComponent->IgnoreCol(ThrowWallProjectile.Get());



	ThrowWallProjectile->FinishSpawning(FTransform::Identity);
}

void ADK_RamPage::DestroyWall()
{
	if (ThrowWallProjectile.IsValid())
	{
		ThrowWallProjectile->DestroyProjectile();
		ThrowWallProjectile = nullptr;
	}
}




bool ADK_RamPage::ThrowWall(bool bIsSpeed)
{
	if (!CanAttack())
	{
		return false;
	}

	// 돌을 안 들고있으면 Failed
	if (!ThrowWallProjectile.IsValid())
		return false;

	ResetInfoOnAttack();

	if(bIsSpeed)
		ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::SpeedThrowWall);
	else
		ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::ThrowWall);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_ThrowWall);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboThrowWall);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboThrowWall);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}

void ADK_RamPage::BindSectionFunction_ThrowWall()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_ThrowWall);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_ThrowWall);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_ThrowWall);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_ThrowWall);
}

void ADK_RamPage::BeginSectionNotify_ThrowWall(FName NotifyName)
{
	if (NotifyName == FName("AttackRange"))
	{
		BeginAttackRange_Notify();
	}
	else if (NotifyName == FName("ColRange"))
	{
		BeginColRange_Notify();
	}
	else if (NotifyName == FName("Throw"))
	{

		if (ThrowWallProjectile.IsValid())
		{
			ThrowWallProjectile->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			ThrowWallProjectile->SimulatingProjectile();

			ThrowWallProjectile = nullptr;
		}

	}

}

void ADK_RamPage::EndSectionNotify_ThrowWall(FName NotifyName)
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

void ADK_RamPage::EndSection_ThrowWall(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::Interrupted_ComboThrowWall()
{
	InterruptedAttack_Notify();

	DestroyWall();

	EndAttack_Notify();
}

void ADK_RamPage::End_ComboThrowWall()
{
	InterruptedAttack_Notify();

	DestroyWall();

	EndAttack_Notify();
}










bool ADK_RamPage::JumpAttack()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::JumpAttack);
	
	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_JumpAttack);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboJumpAttack);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboJumpAttack);

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
		StatComponent->SetIsInvincible(true);

		AActor* Target = AIControllerBase->GetAttackTarget();

		// EndJump 바인딩
		AttackComponent->Delegate_EndJump.Clear();
		AttackComponent->Delegate_EndJump.AddUObject(this, &ADK_RamPage::EndPathJumpAttack);

		AttackComponent->Delegate_StartEndAnim.Clear();
		AttackComponent->Delegate_StartEndAnim.AddUObject(this, &ADK_RamPage::StartEndJumpAttackAnim);

		AttackComponent->JumpToAttackTarget(Target, ComboComponent->GetCurrentAttackInfos().JumpAttackInfo);

	}
	else if (NotifyName == FName("JumpLoop"))
	{
		UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
		
		// End 애니메이션이 호출 중이라면 JumpLoop 안들어옴.
		GetMesh()->GetAnimInstance()->Montage_Pause();

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

void ADK_RamPage::StartEndJumpAttackAnim()
{
	InterruptedAttack_Notify();

	ComboComponent->PlayNextSection();
}

void ADK_RamPage::EndPathJumpAttack()
{
	InterruptedAttack_Notify();

	FVector SlashLocation = GetMesh()->GetSocketLocation(FName("Combo1_Smash"));
	AttackComponent->AOEDamage(SlashLocation, 1500.f, ComboComponent->GetCurrentAttackInfos(), false);

	StatComponent->SetIsInvincible(false);
}


void ADK_RamPage::Interrupted_ComboJumpAttack()
{
	StatComponent->SetIsInvincible(false);
	EndAttack_Notify();
}

void ADK_RamPage::End_ComboJumpAttack()
{
	StatComponent->SetIsInvincible(false);
	EndAttack_Notify();
}







bool ADK_RamPage::BackJump()
{
	if (!CanAttack())
	{
		return false;
	}

	ResetInfoOnAttack();

	ComboComponent->ChangeComboActionData((int8)ERamPage_Attack::BackJump);

	ComboComponent->ClearDelegate();
	ComboComponent->OnSectionEndDelegate.AddUObject(this, &ADK_RamPage::BindSectionFunction_BackJump);
	ComboComponent->OnComboInterruptedDelegate.AddUObject(this, &ADK_RamPage::Interrupted_ComboBackJump);
	ComboComponent->OnComboEndDelegate.AddUObject(this, &ADK_RamPage::End_ComboBackJump);

	ComboComponent->ProcessComboCommand(true);

	bIsAttacking = true;

	return true;
}


void ADK_RamPage::BindSectionFunction_BackJump()
{
	UPlayMontageCallbackProxy* AnimProxy = GetMontageCallbackProxy();
	AnimProxy->OnCompleted.AddDynamic(this, &ADK_RamPage::EndSection_BackJump);
	AnimProxy->OnInterrupted.AddDynamic(this, &ADK_RamPage::EndSection_BackJump);
	AnimProxy->OnNotifyBegin.AddDynamic(this, &ADK_RamPage::BeginSectionNotify_BackJump);
	AnimProxy->OnNotifyEnd.AddDynamic(this, &ADK_RamPage::EndSectionNotify_BackJump);
}


void ADK_RamPage::BeginSectionNotify_BackJump(FName NotifyName)
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

		FVector TargetPos = AIControllerBase->GetLocationOfInterestKey();

		// EndJump 바인딩
		AttackComponent->Delegate_EndJump.Clear();
		AttackComponent->Delegate_EndJump.AddUObject(this, &ADK_RamPage::EndPathBackJump);

		AttackComponent->Delegate_StartEndAnim.Clear();
		AttackComponent->Delegate_StartEndAnim.AddUObject(this, &ADK_RamPage::StartEndBackJumpAnim);

		AttackComponent->JumpToPos(TargetPos, ComboComponent->GetCurrentAttackInfos().JumpAttackInfo);

	}
	else if (NotifyName == FName("JumpLoop"))
	{
		// End 애니메이션이 호출 중이라면 JumpLoop 안들어옴.
		GetMesh()->GetAnimInstance()->Montage_Pause();
	}
}

void ADK_RamPage::EndSectionNotify_BackJump(FName NotifyName)
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

void ADK_RamPage::EndSection_BackJump(FName NotifyName)
{
	InterruptedAttack_Notify();
}

void ADK_RamPage::StartEndBackJumpAnim()
{
	InterruptedAttack_Notify();

	ComboComponent->PlayNextSection();
}

void ADK_RamPage::EndPathBackJump()
{
	InterruptedAttack_Notify();

	StatComponent->SetIsInvincible(false);
}


void ADK_RamPage::Interrupted_ComboBackJump()
{
	StatComponent->SetIsInvincible(false);

	EndAttack_Notify();
}

void ADK_RamPage::End_ComboBackJump()
{
	StatComponent->SetIsInvincible(false);

	EndAttack_Notify();
}

