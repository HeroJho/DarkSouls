// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_ComboComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DARKSOULS_API UDK_ComboComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDK_ComboComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ProcessComboCommand();
	void ComboCheck();

	void ChangeComboActionData(uint8 DataIndex);

protected:	
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);

	virtual void NotifyComboActionEnd();

protected:
	UPROPERTY()
	TObjectPtr<class ADK_Creature> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UDK_ComboActionData>> ComboActionDatas;

	uint8 CurComboActionDataIndex = 0;
	int8 ReserveComboActionDataIndex = -1;

	int32 CurrentCombo = 0;
	bool HasNextComboCommand = false;
	bool IsChangedData = false;

		
};
