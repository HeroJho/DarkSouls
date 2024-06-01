// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_CollisionManagerComponent.generated.h"


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
	UFUNCTION(BlueprintCallable)
	void TurnAttackCol(const TArray<FString>& CapsuleNames, bool bAutoSetAnotherBlock = false);
	UFUNCTION(BlueprintCallable)
	void TurnBlockCol(const TArray<FString>& CapsuleNames);
	UFUNCTION(BlueprintCallable)
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FString, TObjectPtr<class UCapsuleComponent>> Capsules;

	UPROPERTY()
	TSoftObjectPtr<class ADK_Creature> CreatureOwner;

	UPROPERTY()
	TSet<TSoftObjectPtr<class AActor>> ActorTemps;

};
