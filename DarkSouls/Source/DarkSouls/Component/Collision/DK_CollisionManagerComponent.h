// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/DK_DamageableInterface.h"
#include "DK_CollisionManagerComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FOnColHitDelegete, IDK_DamageableInterface*);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_CollisionManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_CollisionManagerComponent();

protected:
	virtual void InitializeComponent() override;

	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	void TurnAttackCol(const TArray<FString>& CapsuleNames, bool bAutoSetAnotherBlock = false);
	void TurnBlockCol(const TArray<FString>& CapsuleNames);
	void TurnBlockAllCol();
	
	bool CheckIsAttackCol(FString Name);

	void ClearActorTemps();

protected:
	bool CheckAttackedActor(AActor* InCreature);

	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	UPROPERTY(VisibleAnywhere)
	TMap<FString, TWeakObjectPtr<class UCapsuleComponent>> Capsules;

	UPROPERTY()
	TObjectPtr<class ADK_Object> CreatureOwner;

	UPROPERTY()
	TSet<TSoftObjectPtr<class AActor>> ActorTemps;


	// Delegate
public:
	FOnColHitDelegete OnColHitDelegate;


};
