// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DK_ToolManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"


#include "Tool/Define.h"
#include "Game/DK_GameMode.h"

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

			DrawDebugSphere(GetWorld(), Location, 150.f, 20, FColor::Red, false, 10.f);
		}
	}

#endif 



	return Answers;
}

AActor* UDK_ToolManager::GetObjectInNearstAngleByChannel(ECollisionChannel Channel, FVector StanPos, float MaxDis, FVector StanVec, float MaxAngle, bool bDebug)
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
			DrawDebugSphere(GetWorld(), Answer->GetActorLocation(), 150.f, 20, FColor::Green, false, 10.f);
		}
	}

#endif 


	return Answer;
}

FVector UDK_ToolManager::CalculateFutureActorXYLocation(AActor* Target, float Time)
{
	FVector Location = Target->GetActorLocation();
	FVector Velocity = Target->GetVelocity();
	Velocity.Z = 0.f;


	FVector FutureLocation = Location + (Velocity * Time);

	return FutureLocation;
}





bool UDK_ToolManager::PredictProjectilePath(AActor* Me, AActor* Target, TArray<FVector>& OUT_Pos, float PredictTime, float Arc, float FrontDis, bool bDebug)
{
	FVector OwnerLocation = Me->GetTargetLocation();
	FVector FutureLocation = CalculateFutureActorXYLocation(Target, PredictTime);
	FutureLocation.Z = 100.f;


	if (0.f < FrontDis)
	{
		FVector DisVec = Me->GetActorLocation() - FutureLocation;
		if (DisVec.Length() > FrontDis * 4.f)
		{
			DisVec.Normalize();
			FutureLocation += DisVec * FrontDis;
		}
	}


	FVector OutLaunchVelocity;

	bool bResult = UGameplayStatics::SuggestProjectileVelocity_CustomArc(
		Me,
		OutLaunchVelocity,
		OwnerLocation,
		FutureLocation,
		0.f,
		Arc
	);


	if (bResult == false)
		return false;


	// 20: tracing 보여질 프로젝타일 크기, 15: 시물레이션되는 Max 시간(초)
	FPredictProjectilePathParams predictParams(20.0f, OwnerLocation, OutLaunchVelocity, 15.0f);
	
	predictParams.OverrideGravityZ = GetWorld()->GetGravityZ();
	predictParams.SimFrequency = 5.f;

	if (bDebug)
	{
		predictParams.DrawDebugTime = 5.0f;     //디버그 라인 보여지는 시간 (초)
		predictParams.DrawDebugType = EDrawDebugTrace::ForDuration;
	}
	else
		predictParams.DrawDebugType = EDrawDebugTrace::None;



	predictParams.bTraceWithCollision = true;
	predictParams.TraceChannel = ECollisionChannel::ECC_Camera;
	

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(this, predictParams, Result);


	for (int32 i = 0; i < Result.PathData.Num(); ++i)
	{
		OUT_Pos.Add(Result.PathData[i].Location);
	}


	return true;
}
