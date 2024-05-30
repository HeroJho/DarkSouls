// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Engine/DamageEvents.h"

#include "Tool/Define.h"
#include "Components/CapsuleComponent.h"
#include "Creature/DK_Creature.h"

// Sets default values for this component's properties
UDK_CollisionManagerComponent::UDK_CollisionManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;

	// ...
}


void UDK_CollisionManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();


	CreatureOwner = Cast<ADK_Creature>(GetOwner());

	// Store All CapsuleCollisions
	TArray<UCapsuleComponent*> Components;
	CreatureOwner->GetComponents(UCapsuleComponent::StaticClass(), Components);

	for (int32 i = 0; i < Components.Num(); ++i)
	{
		FString Name = Components[i]->GetName();
	
		if (Name == TEXT("CollisionCylinder"))
			continue;

		Capsules.Add(Name, Components[i]);
		
		FScriptDelegate BeginDelegate;
		BeginDelegate.BindUFunction(this, FName("OnOverlapBegin"));
		Components[i]->OnComponentBeginOverlap.AddUnique(BeginDelegate);
		FScriptDelegate EndDelegate;
		EndDelegate.BindUFunction(this, FName("OnOverlapEnd"));
		Components[i]->OnComponentEndOverlap.AddUnique(EndDelegate);

		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		//	FString::Printf(TEXT("%s"), *Name));
	}

}

// Called when the game starts
void UDK_CollisionManagerComponent::BeginPlay()
{
	Super::BeginPlay();



}


// Called every frame
void UDK_CollisionManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDK_CollisionManagerComponent::TurnAttackCol(const TArray<FString>& CapsuleNames, bool bAutoSetAnotherBlock)
{
	if (bAutoSetAnotherBlock)
	{
		for (auto Iter : Capsules)
		{
			Iter.Value->SetCollisionProfileName(COL_BLOCK);
		}
	}

	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_ATTACK);

		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		FString::Printf(TEXT("%s"), *CapsuleNames[i]));
	}

}

void UDK_CollisionManagerComponent::TurnBlockCol(const TArray<FString>& CapsuleNames)
{
	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_BLOCK);
	}
}

void UDK_CollisionManagerComponent::TurnBlockAllCol()
{
	for (auto Iter : Capsules)
	{
		Iter.Value->SetCollisionProfileName(COL_BLOCK);
	}
}

bool UDK_CollisionManagerComponent::CheckIsAttackCol(FString Name)
{
	TObjectPtr<UCapsuleComponent>* Capsule = Capsules.Find(Name);
	
	if (Capsule)
	{
		if ((*Capsule)->GetCollisionProfileName() == COL_ATTACK)
		{
			return true;
		}
	}

	return false;
}

void UDK_CollisionManagerComponent::ClearCreatureTemps()
{
	CreatureTemps.Empty();
}


bool UDK_CollisionManagerComponent::CheckAttackedCreature(AActor* InCreature)
{
	TSoftObjectPtr<AActor>* Creature = CreatureTemps.Find(InCreature);

	if (Creature)
		return true;

	return false;
}

void UDK_CollisionManagerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == GetOwner())
		return;

	// 공격 애니메이션 범위인지
	if (!CreatureOwner->IsInAttackRange())
		return;

	FString ColName = OverlappedComp->GetName();
	// 공격 판정 콜라이더라면 
	if (CheckIsAttackCol(ColName))
	{
		// 중복 공격 방지
		if (CheckAttackedCreature(OtherActor))
			return;
		CreatureTemps.Add(OtherActor);

		// 대미지
		FDamageEvent DamageEvent;
		// Instigator 누가 대미지를 입혔는가, Causer 무엇이 대미지를 입혔는가
		OtherActor->TakeDamage(0, DamageEvent, CreatureOwner->GetController(), OtherActor);
	}

}

void UDK_CollisionManagerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner())
		return;

}

