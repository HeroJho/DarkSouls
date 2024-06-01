// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"

// Sets default values
ADK_Creature::ADK_Creature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	// GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"));

	// Mesh
	// GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;


	// ComboComponent
	ComboComponent = CreateDefaultSubobject<UDK_ComboComponent>(TEXT("ComboComponent"));

	// CollisionManagerComponent
	CollisionManagerComponent = CreateDefaultSubobject<UDK_CollisionManagerComponent>("CollisionManagerComponent");

}

// Called when the game starts or when spawned
void ADK_Creature::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADK_Creature::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADK_Creature::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}






void ADK_Creature::Attack()
{
	ComboComponent->ProcessComboCommand();
}

void ADK_Creature::BeginAttackRange_Notify()
{
	bIsInAttackRange = true;
}

void ADK_Creature::EndAttackRange_Notify()
{
	bIsInAttackRange = false;
	CollisionManagerComponent->ClearActorTemps();
}

void ADK_Creature::BeginColRange_Notify()
{
	const TArray<FString>& AttackCollisionInfos = ComboComponent->GetCurrentAttackCollisionInfos();
	CollisionManagerComponent->TurnAttackCol(AttackCollisionInfos, true);
}

void ADK_Creature::EndColRange_Notify()
{
	CollisionManagerComponent->TurnBlockAllCol();
}

void ADK_Creature::GetCurrentAttackInfos(float& OUT_Damage, bool& OUT_bIsDown, bool& OUT_bSetStunTimeToHitAnim, float& OUT_StunTime)
{
	ComboComponent->GetCurrentAttackInfos(OUT_Damage, OUT_bIsDown, OUT_bSetStunTimeToHitAnim, OUT_StunTime);
}



//float ADK_Creature::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
//{
//	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//
//	OnDamaged();
//
//	return 0.0f;
//}

void ADK_Creature::OnDamaged(float DamageAmount, bool bIsDown, bool bSetStunTimeToHitAnim, float StunTime, AActor* DamageCauser)
{
	// GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Blue, FString::Printf(TEXT("%s is Attacked"), *GetName()));
	
	if (!bIsDown)
	{
		if (bSetStunTimeToHitAnim)
		{
			const int32 HitAnimMontageIndex = 0;
			float StartTime = 0.f, EndTime = 0.f;
			HitMontage->GetSectionStartAndEndTime(HitAnimMontageIndex, StartTime, EndTime);

			float DisTime = EndTime - StartTime - 0.1f;

			Stun(DisTime);
		}
		else
		{
			Stun(StunTime);
		}


	}
	else
	{
		// TODO

	}

}








void ADK_Creature::Stun(float StunTime)
{
	ResetInfoOnStun();

	bIsStun = true;

	GetWorldTimerManager().ClearTimer(StunTimerHandle);
	GetWorldTimerManager().SetTimer(StunTimerHandle, this, &ADK_Creature::EndStun, StunTime, false);
	
	if (HitMontage)
	{
		PlayAnimMontage(HitMontage, 1.f);
	}
	
}

void ADK_Creature::ResetInfoOnStun()
{
	EndColRange_Notify(); // 공격 콜라이더
	EndAttackRange_Notify(); // 공격 애니메이션 범위
	ComboComponent->ResetComboInfo(); // 콤보
}

void ADK_Creature::EndStun()
{
	bIsStun = false;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(AnimInstance->Montage_IsPlaying(HitMontage))
		StopAnimMontage(HitMontage);
}
