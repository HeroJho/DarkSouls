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
		if (Components[i]->ComponentHasTag(TEXT("NoManage")))
			continue;

	
		FString Name = Components[i]->GetName();

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
		TurnBlockAllCol();
	}

	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_ATTACK);
		// Capsules[CapsuleNames[i]]->bHiddenInGame = false;
	}

}

void UDK_CollisionManagerComponent::TurnBlockCol(const TArray<FString>& CapsuleNames)
{
	for (int32 i = 0; i < CapsuleNames.Num(); ++i)
	{
		Capsules[CapsuleNames[i]]->SetCollisionProfileName(COL_BLOCK);
		// Capsules[CapsuleNames[i]]->bHiddenInGame = true;
	}
}

void UDK_CollisionManagerComponent::TurnBlockAllCol()
{
	for (auto Iter : Capsules)
	{
		if (Iter.Value->ComponentHasTag(TEXT("NoBlock")))
			continue;

		Iter.Value->SetCollisionProfileName(COL_BLOCK);
		// Iter.Value->bHiddenInGame = true;
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

void UDK_CollisionManagerComponent::ClearActorTemps()
{
	ActorTemps.Empty();
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
	if (OtherActor == GetOwner())
		return;

	// ���� �ִϸ��̼� ��������
	if (!CreatureOwner->IsInAttackRange())
		return;

	FString ColName = OverlappedComp->GetName();
	// ���� ���� �ݶ��̴���� 
	if (CheckIsAttackCol(ColName))
	{
		// �ߺ� ���� ����
		if (CheckAttackedActor(OtherActor))
			return;
		ActorTemps.Add(OtherActor);

		// �����
		//FDamageEvent DamageEvent;
		// Instigator ���� ������� �����°�, Causer ������ ������� �����°�
		//OtherActor->TakeDamage(0, DamageEvent, CreatureOwner->GetController(), OtherActor);

		ADK_Creature* OtherCreature = Cast<ADK_Creature>(OtherActor);
		float Damage = 0.f;
		bool bIsDown = false;
		bool bSetStunTimeToHitAnim = false;
		float StunTime = 0.f;
		CreatureOwner->GetCurrentAttackInfos(Damage, bIsDown, bSetStunTimeToHitAnim, StunTime);

		OtherCreature->OnDamaged(Damage, bIsDown, bSetStunTimeToHitAnim, StunTime, CreatureOwner);

/*		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Green,
		FString::Printf(TEXT("%f, %d, %f"), Damage, bIsDown, StunTime))*/;
	}

}

void UDK_CollisionManagerComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == GetOwner())
		return;



}

