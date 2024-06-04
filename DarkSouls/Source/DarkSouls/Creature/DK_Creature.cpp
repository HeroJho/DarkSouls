// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/DK_Creature.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Tool/Define.h"
#include "Component/Combo/DK_ComboComponent.h"
#include "Component/Collision/DK_CollisionManagerComponent.h"

// Sets default values
ADK_Creature::ADK_Creature()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("RootCreatureCol"));

	// Mesh
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
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

	// 타이머로 할려고했는데, 카메라가 흔들리는 문제가 생김
	if (bIsSmoothTurn)
	{
		SmoothTurnTick();
	}


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





void ADK_Creature::SmoothTurnByCallOnce(FVector InDestPos, float TurnSpeed)
{
	if (!CanSmoothTurn())
		return;

	DestPos = InDestPos;
	SmoothTurnSpeed = TurnSpeed;

	bIsSmoothTurn = true;
}

void ADK_Creature::StopSmoothTurn()
{
	bIsSmoothTurn = false;
}

bool ADK_Creature::CanSmoothTurn()
{
	if (bIsStun)
		return false;


	return true;
}

void ADK_Creature::SmoothTurnTick()
{
	if (!CanSmoothTurn())
	{
		StopSmoothTurn();
		return;
	}


	FRotator CurRot = K2_GetActorRotation();
	CurRot.Roll = 0.f;
	CurRot.Pitch = 0.f;
	FRotator DestRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), DestPos);
	DestRot.Roll = 0.f;
	DestRot.Pitch = 0.f;

	FRotator NowRot = FMath::RInterpTo(CurRot, DestRot, GetWorld()->DeltaTimeSeconds, SmoothTurnSpeed);

	FRotator DisRot = CurRot - NowRot;
	if (DisRot.IsNearlyZero(0.1f))
	{
		StopSmoothTurn();
		return;
	}

	SetActorRotation(NowRot);
	
}



void ADK_Creature::Dodge()
{
	bIsDodge = true;

	PlayAnimMontage(DodgeMontage);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ADK_Creature::EndDoge);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
}

void ADK_Creature::EndDoge(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsDodge = false;


}

void ADK_Creature::StartDodgeSkip_Notify()
{
	CollisionManagerComponent->TurnDodgeCol();

}

void ADK_Creature::EndDodgeSkip_Notify()
{
	CollisionManagerComponent->TurnBlockAllCol();

}
