// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DK_ToolManager.h"

#include "Tool/Define.h"

// Sets default values for this component's properties
UDK_ToolManager::UDK_ToolManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDK_ToolManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

const TArray<AActor*> UDK_ToolManager::GetObjectInAngleByChannel(ECollisionChannel Channel, FVector StanPos, float MaxDis, FVector StanVec, float MaxAngle, bool bDebug)
{
	TArray<AActor*> Answers;

	// 트레이스로 오브젝트를 얻어온다
	TArray<FOverlapResult> TraceHitArray;
	GetWorld()->OverlapMultiByChannel(TraceHitArray, StanPos, FQuat::Identity, Channel, FCollisionShape::MakeSphere(MaxDis));


	// 내적해서 MaxAngle안에 있다
	for (int32 i = 0; i < TraceHitArray.Num(); ++i)
	{
		AActor* TargetTemp = TraceHitArray[i].GetActor();
		if (!IsValid(TargetTemp))
			continue;
		
		FVector LookToTargetVector = TargetTemp->GetActorLocation() - StanPos;
		LookToTargetVector.Normalize();
		StanVec.Normalize();

		float  DotResult = FVector::DotProduct(StanVec, LookToTargetVector);
		
		float Angle =  (FMath::Acos(DotResult) / UE_PI) * 180.f;
		
		if (FMath::Abs(Angle) <= MaxAngle)
		{
			Answers.Push(TargetTemp);
		}

	}


#ifdef UE_BUILD_DEBUG

	if (bDebug)
	{
		DrawDebugSphere(GetWorld(), StanPos, MaxDis, 12, FColor::White);

		for (int32 i = 0; i < Answers.Num(); ++i)
		{
			FVector Location = Answers[i]->GetActorLocation();

			DrawDebugSphere(GetWorld(), Location, 150.f, 20, FColor::Red);
		}
	}

#endif 



	return Answers;
}

const AActor* UDK_ToolManager::GetObjectInNearstAngleByChannel(ECollisionChannel Channel, FVector StanPos, float MaxDis, FVector StanVec, float MaxAngle, bool bDebug)
{
	AActor* Answer = nullptr;

	TArray<AActor*> Targets;
	Targets = GetObjectInAngleByChannel(Channel, StanPos, MaxDis, StanVec, MaxAngle, bDebug);

	float MinRad = 0.f;
	for (int32 i = 0; i < Targets.Num(); ++i)
	{
		FVector LookToTargetVector = Targets[i]->GetTargetLocation() - StanPos;
		LookToTargetVector.Normalize();
		
		StanVec.Normalize();

		float Rad = FVector::DotProduct(StanVec, LookToTargetVector);

		if (FMath::Abs(Rad) >= MinRad)
		{
			MinRad = Rad;
			Answer = Targets[i];
		}

	}


#ifdef UE_BUILD_DEBUG

	if (bDebug)
	{
		if (Answer)
		{
			DrawDebugSphere(GetWorld(), Answer->GetActorLocation(), 150.f, 20, FColor::Green);
		}
	}

#endif 


	return Answer;
}
