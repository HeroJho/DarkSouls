// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DK_Projectile_Gravity.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Creature/DK_Object.h"

ADK_Projectile_Gravity::ADK_Projectile_Gravity()
{
	// PrimaryActorTick.bCanEverTick = false;

	PitchDegree = -1.f;
}

void ADK_Projectile_Gravity::BeginPlay()
{
	Super::BeginPlay();

}


void ADK_Projectile_Gravity::RotateToTarget()
{
	// bRotationFollowsVelocity Option is true;
	if (TargetActor.IsValid() == false)
		return;

	FVector TargetLocation = TargetActor->GetActorLocation();

	ADK_Object* ObjTarget = Cast<ADK_Object>(TargetActor);
	if (IsValid(ObjTarget))
	{
		TargetLocation = ObjTarget->GetMiddlePos();
	}


	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation);
	
	if (UKismetMathLibrary::NearlyEqual_FloatFloat(PitchDegree, 0.f) == false)
	{
		UnitVector = UnitVector.RotateAngleAxis(PitchDegree, GetOwner()->GetActorRightVector());
	}
	
	UnitVector *= Speed;

	ProjectileMovementComponent->Velocity = UnitVector;
}



