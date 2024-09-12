// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Attack/DK_AttackComponent.h"
#include "GameFramework/Character.h"
#include "Curves/CurveFloat.h"
#include "Kismet/KismetMathLibrary.h"

#include "Struct/S_DamageInfo.h"
#include "Interface/DK_DamageableInterface.h"
#include "AOE/DK_AOE_Base.h"
#include "Game/DK_GameMode.h"
#include "Manager/DK_ToolManager.h"



FS_JumpAttackInfo::FS_JumpAttackInfo()
	: Curve(nullptr), JumpSpeed(0.f),
	PredictTime(0.f), Arc(0.f), MinDisRange(0.f), MaxDisRange(0.f), MinArc(0.f), MaxArc(0.f),
	bIsFrontTarget(false), FrontDis(0.f), bRenderDebug(false)
{
}

FS_JumpAttackInfo::FS_JumpAttackInfo(UCurveFloat* In_Curve, float In_JumpSpeed, float In_PredictTime,
	float In_Arc, float In_MinDisRange, float In_MaxDisRange, float In_MinArc, float In_MaxArc,
	bool In_bIsFrontTarget, float In_FrontDis, bool In_bRenderDebug)
	: Curve(In_Curve), JumpSpeed(In_JumpSpeed), 
	PredictTime(In_PredictTime), Arc(In_Arc), MinDisRange(In_MinDisRange), MaxDisRange(In_MaxDisRange), MinArc(In_MinArc), MaxArc(In_MaxArc),
	bIsFrontTarget(In_bIsFrontTarget), FrontDis(In_FrontDis), bRenderDebug(In_bRenderDebug)
{

}


// Sets default values for this component's properties
UDK_AttackComponent::UDK_AttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_AttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CharacterOwner = Cast<ACharacter>(GetOwner());
}

void UDK_AttackComponent::AOEDamage(FVector SpawnLocation, float Radius, FS_DamageInfo DamageInfo, bool bIsRenderDebug)
{
	const FTransform SpawnTransform(SpawnLocation);
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	ADK_AOE_Base* SlashAOE = GetWorld()->SpawnActorDeferred<ADK_AOE_Base>(ADK_AOE_Base::StaticClass(), SpawnTransform, OwnerPawn, OwnerPawn);
	SlashAOE->InitOption(300.f, bIsRenderDebug, true, true);

	SlashAOE->OnAOEOverlapActorDelegate.AddLambda([DamageInfo, OwnerPawn](AActor* HitActor)
		{
			if (!IsValid(OwnerPawn))
				return;

			IDK_DamageableInterface* HitActorDamageable = Cast<IDK_DamageableInterface>(HitActor);
			if (HitActorDamageable)
			{
				HitActorDamageable->TakeDamage(DamageInfo, OwnerPawn);
			}
		});

	SlashAOE->FinishSpawning(SpawnTransform);


	NewObject<ADK_AOE_Base>();

}





bool UDK_AttackComponent::JumpToAttackTarget(AActor* Target, FS_JumpAttackInfo JumpAttackInfo)
{
	// Init
	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	JumpDeltaTimeAcc = 0.f;

	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());


	// Getting Path of Jump
	TArray<FVector> Poss;
	
	float Arc = JumpAttackInfo.Arc;
	if (Arc <= 0.f)
	{
		float Distance = CharacterOwner->GetDistanceTo(Target);
		float Rad = UKismetMathLibrary::FClamp(UKismetMathLibrary::NormalizeToRange(Distance, JumpAttackInfo.MinDisRange, JumpAttackInfo.MaxDisRange), 0.f, 1.f);
		Arc = UKismetMathLibrary::Lerp(JumpAttackInfo.MinArc, JumpAttackInfo.MaxArc, Rad);
	}
	
	GameMode->GetToolManager()->PredictProjectilePath(GetOwner(), Target, Poss, JumpAttackInfo.PredictTime, 
		Arc, JumpAttackInfo.bIsFrontTarget, JumpAttackInfo.FrontDis, JumpAttackInfo.bRenderDebug);

	float EndTime = CharacterOwner->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(2);


	// Run JumpTick
	FTimerDelegate JumpTimerDelegate;
	JumpTimerDelegate.BindUFunction(this, FName("JumpTick"), JumpAttackInfo.Curve, Poss, EndTime, JumpAttackInfo.JumpSpeed);
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 0.01f, true);


	return true;
}

void UDK_AttackComponent::JumpTick(UCurveFloat* Curve, TArray<FVector> Poss, float EndAnimLength, float JumpSpeed)
{
	const float TimeDelta = GetWorld()->DeltaTimeSeconds;

	// 0~1 누적
	JumpDeltaTimeAcc = FMath::Clamp(JumpDeltaTimeAcc + TimeDelta * JumpSpeed, 0.f, 1.f);


	// EndAnim 실행 위치 구하기
	float TotalTime = 1.f / JumpSpeed;
	float EndRatio = EndAnimLength / TotalTime;

	// EndAnim 실행
	UAnimInstance* OwnerAnim = CharacterOwner->GetMesh()->GetAnimInstance();
	if (OwnerAnim->Montage_GetIsStopped(nullptr) && (1.f - EndRatio <= JumpDeltaTimeAcc))
	{
		OwnerAnim->Montage_Resume(nullptr);
	}


	// Curve 샘플링
	float CurveValue = Curve->GetFloatValue(JumpDeltaTimeAcc);

	// CurveValue에 해당하는 앞뒤 인덱스
	int CurIndex = CurveValue * Poss.Num();
	int NexIndex = CurIndex + 1;


	// 끝
	// *JumpDeltaTimeAcc는 1에 꼭 도달하기에 끝을 보장한다
	// 루프가 걸리기전에 호출
	if (NexIndex >= Poss.Num())
	{
		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
		Delegate_EndJump.Broadcast();

		if (OwnerAnim->Montage_GetIsStopped(nullptr))
			OwnerAnim->Montage_Resume(nullptr);

		return;
	}


	// 두 위치 보간 적용  
	float LerpAcc = (CurveValue * Poss.Num()) - CurIndex;
	FVector DestPos = UKismetMathLibrary::VLerp(Poss[CurIndex], Poss[NexIndex], LerpAcc);

	GetOwner()->SetActorLocation(DestPos, true);
}


