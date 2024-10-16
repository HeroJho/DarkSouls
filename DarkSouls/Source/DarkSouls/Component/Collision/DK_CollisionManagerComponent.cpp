// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Collision/DK_CollisionManagerComponent.h"
#include "Engine/DamageEvents.h"

#include "Tool/Define.h"
#include "Components/CapsuleComponent.h"
#include "Creature/DK_Object.h"

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


	CreatureOwner = Cast<ADK_Object>(GetOwner());

	// Store All CapsuleCollisions
	TArray<UCapsuleComponent*> Components;
	CreatureOwner->GetComponents(UCapsuleComponent::StaticClass(), Components);

	for (int32 i = 0; i < Components.Num(); ++i)
	{
		if (Components[i]->ComponentHasTag(COLTAG_NOMANAGE))
			continue;
	
		FString Name = Components[i]->GetName();

		Capsules.Add(Name, Components[i]);
		
		FScriptDelegate BeginDelegate;
		BeginDelegate.BindUFunction(this, FName("OnOverlapBegin"));
		Components[i]->OnComponentBeginOverlap.AddUnique(BeginDelegate);
		FScriptDelegate EndDelegate;
		EndDelegate.BindUFunction(this, FName("OnOverlapEnd"));
		Components[i]->OnComponentEndOverlap.AddUnique(EndDelegate);

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
		TurnBlockAllCol();
	}

	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		if (!Capsules[CapsuleNames[i]].IsValid())
			continue;

		Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_ATTACK);
		// Capsules[CapsuleNames[i]]->bHiddenInGame = false;

		//GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		//FString::Printf(TEXT("%s"), *Capsules[CapsuleNames[i]].GetName()));
	}

}

void UDK_CollisionManagerComponent::TurnBlockCol(const TArray<FString>& CapsuleNames)
{
	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		if (!Capsules[CapsuleNames[i]].IsValid())
			continue;

		if (Capsules[CapsuleNames[i]]->ComponentHasTag(COLTAG_ONLYATTACK))
			continue;


		if (Capsules[CapsuleNames[i]]->ComponentHasTag(COLTAG_NOBLOCK))
		{
			Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_NONBLOCK);
		}
		else
		{
			Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_BLOCK);
		}

		// Capsules[CapsuleNames[i]]->bHiddenInGame = true;
	}
}

void UDK_CollisionManagerComponent::TurnBlockAllCol()
{
	for (auto Iter : Capsules)
	{
		if (!Iter.Value.IsValid())
			continue;

		if (Iter.Value->ComponentHasTag(COLTAG_ONLYATTACK))
			continue;


		if (Iter.Value->ComponentHasTag(COLTAG_NOBLOCK))
		{
			Iter.Value->SetCollisionProfileName(COL_NONBLOCK);
		}
		else
		{
			Iter.Value->SetCollisionProfileName(COL_BLOCK);
		}

	}
}

bool UDK_CollisionManagerComponent::CheckIsAttackCol(FString Name)
{
	TWeakObjectPtr<UCapsuleComponent>* Capsule = Capsules.Find(Name);
	
	if (Capsule != nullptr && (*Capsule).IsValid())
	{
		const FName& ProfileName = (*Capsule)->GetCollisionProfileName();
		if (ProfileName == COL_ATTACK || ProfileName == COL_NONBLOCK)
		{
			return true;
		}
	}

	return false;
}

void UDK_CollisionManagerComponent::ClearActorTemps()
{
	ActorTemps.Empty();
}

void UDK_CollisionManagerComponent::IgnoreCol(AActor* IgnoreActor)
{
	TArray<UCapsuleComponent*> Components;
	CreatureOwner->GetComponents(UCapsuleComponent::StaticClass(), Components);

	for (int32 i = 0; i < Components.Num(); ++i)
	{
		Components[i]->IgnoreActorWhenMoving(IgnoreActor, true);
	}
}



bool UDK_CollisionManagerComponent::CheckAttackedActor(AActor* InCreature)
{
	TSoftObjectPtr<AActor>* Creature = ActorTemps.Find(InCreature);

	if (Creature)
		return true;

	return false;
}

void UDK_CollisionManagerComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 자신을 충돌했다면
	if (OtherActor == GetOwner())
		return;

	// 데미지 인터페이스 여부
	IDK_DamageableInterface* OtherDamageable = Cast<IDK_DamageableInterface>(OtherActor);
	if (OtherDamageable == nullptr)
		return;

	// 공격 애니메이션 범위인지
	if (!CreatureOwner->IsInAttackRange())
		return;

	// 공격 판정 콜라이더가 아니라면 
	FString ColName = OverlappedComp->GetName();
	if (!CheckIsAttackCol(ColName))
		return;

	// 중복 공격 방지
	if (CheckAttackedActor(OtherActor))
		return;
	ActorTemps.Add(OtherActor);


	OnColHitDelegate.Broadcast(OtherDamageable);

}

void UDK_CollisionManagerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner())
		return;



}

