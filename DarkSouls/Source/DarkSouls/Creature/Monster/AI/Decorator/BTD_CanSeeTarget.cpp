// Fill out your copyright notice in the Description page of Project Settings.


#include "Creature/Monster/AI/Decorator/BTD_CanSeeTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTD_CanSeeTarget::UBTD_CanSeeTarget()
{
	NodeName = TEXT("CanSeeTarget");

}

bool UBTD_CanSeeTarget::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AActor* Owner = Cast<AActor>(OwnerComp.GetAIOwner()->GetPawn());
	if (IsValid(Owner) == false)
		return false;

	AActor* Target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName));
	if (IsValid(Target) == false)
		return false;
	

	FHitResult HitResult;
	FCollisionQueryParams ColQueryPar;
	ColQueryPar.AddIgnoredActor(Owner);

	bool bIsHit = GetWorld()->LineTraceSingleByChannel(HitResult, Owner->GetActorLocation(), Target->GetActorLocation(), ECollisionChannel::ECC_Visibility, ColQueryPar);

	if (bIsHit == false)
		return false;

	if (Target == HitResult.GetActor())
		return true;


	return false;
}