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
	bTriggerStartEndAnim = false;

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
		Arc, JumpAttackInfo.FrontDis, JumpAttackInfo.bRenderDebug);

	float EndTime = CharacterOwner->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(2);

	// Run JumpTick
	FTimerDelegate JumpTimerDelegate;
	JumpTimerDelegate.BindUFunction(this, FName("JumpTick"), JumpAttackInfo.Curve, Poss, EndTime, JumpAttackInfo.JumpSpeed, JumpAttackInfo.EndAnimPlayRatio);
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 0.01f, true, 0.f);


	return true;
}

bool UDK_AttackComponent::JumpToPos(FVector Pos, FS_JumpAttackInfo JumpAttackInfo)
{
	// Init
	GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
	JumpDeltaTimeAcc = 0.f;
	bTriggerStartEndAnim = false;

	ADK_GameMode* GameMode = Cast<ADK_GameMode>(GetWorld()->GetAuthGameMode());


	// Getting Path of Jump
	TArray<FVector> Poss;

	FVector OwnerLocation = CharacterOwner->GetActorLocation();
	float Arc = JumpAttackInfo.Arc;
	if (Arc <= 0.f)
	{
		float Distance = UKismetMathLibrary::Vector_Distance(OwnerLocation, Pos);
		float Rad = UKismetMathLibrary::FClamp(UKismetMathLibrary::NormalizeToRange(Distance, JumpAttackInfo.MinDisRange, JumpAttackInfo.MaxDisRange), 0.f, 1.f);
		Arc = UKismetMathLibrary::Lerp(JumpAttackInfo.MinArc, JumpAttackInfo.MaxArc, Rad);
	}

	GameMode->GetToolManager()->PredictProjectilePath(OwnerLocation, Pos, Poss, Arc, JumpAttackInfo.bRenderDebug);

	float EndTime = CharacterOwner->GetMesh()->GetAnimInstance()->GetCurrentActiveMontage()->GetSectionLength(2);

	// Run JumpTick
	FTimerDelegate JumpTimerDelegate;
	JumpTimerDelegate.BindUFunction(this, FName("JumpTick"), JumpAttackInfo.Curve, Poss, EndTime, JumpAttackInfo.JumpSpeed, JumpAttackInfo.EndAnimPlayRatio);
	GetWorld()->GetTimerManager().SetTimer(JumpTimerHandle, JumpTimerDelegate, 0.01f, true, 0.f);


	return true;
}


void UDK_AttackComponent::JumpTick(UCurveFloat* Curve, TArray<FVector> Poss, float EndAnimLength, float JumpSpeed, float EndAnimPlayRatio)
{
	const float TimeDelta = GetWorld()->DeltaTimeSeconds;

	// 0~1 ����
	JumpDeltaTimeAcc = FMath::Clamp(JumpDeltaTimeAcc + TimeDelta * JumpSpeed, 0.f, 1.f);

	// EndAnim ���� ��ġ ���ϱ�
	float TotalTime = 1.f / JumpSpeed;
	float EndRatio = EndAnimLength / TotalTime;
	if (EndAnimPlayRatio >= 0.f)
		EndRatio = 1.f - EndAnimPlayRatio;

	// EndAnim ����
	if (1.f - EndRatio <= JumpDeltaTimeAcc)
	{
		// ���� �ɷȴٸ�
		UAnimInstance* OwnerAnim = CharacterOwner->GetMesh()->GetAnimInstance();
		if (OwnerAnim->Montage_GetIsStopped(nullptr))
			OwnerAnim->Montage_Resume(nullptr);

		if (false == bTriggerStartEndAnim)
		{
			bTriggerStartEndAnim = true;
			Delegate_StartEndAnim.Broadcast();
		}
		
	}


	// Curve ���ø�
	float CurveValue = Curve->GetFloatValue(JumpDeltaTimeAcc);

	// CurveValue�� �ش��ϴ� �յ� �ε���
	int CurIndex = CurveValue * Poss.Num();
	int NexIndex = CurIndex + 1;

	// ��
	// *JumpDeltaTimeAcc�� 1�� �� �����ϱ⿡ ���� �����Ѵ�
	// ������ �ɸ������� ȣ��
	if (NexIndex >= Poss.Num())
	{
		UAnimInstance* OwnerAnim = CharacterOwner->GetMesh()->GetAnimInstance();
		if (OwnerAnim->Montage_GetIsStopped(nullptr))
			OwnerAnim->Montage_Resume(nullptr);
		// Poss�� ������ ������ ������ �̺κ��� ���� ȣ��� ���� ������
		if (false == bTriggerStartEndAnim)
		{
			bTriggerStartEndAnim = true;
			Delegate_StartEndAnim.Broadcast();
		}

		GetWorld()->GetTimerManager().ClearTimer(JumpTimerHandle);
		Delegate_EndJump.Broadcast();

		return;
	}


	// �� ��ġ ���� ����  
	float LerpAcc = (CurveValue * Poss.Num()) - CurIndex;
	FVector DestPos = UKismetMathLibrary::VLerp(Poss[CurIndex], Poss[NexIndex], LerpAcc);

	GetOwner()->SetActorLocation(DestPos, true);
}


