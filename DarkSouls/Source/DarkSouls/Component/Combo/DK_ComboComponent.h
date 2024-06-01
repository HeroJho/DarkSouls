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
	void ProcessComboCommand();
	void ComboCheck();

	UFUNCTION(BlueprintCallable)
	void ChangeComboActionData(uint8 DataIndex);

	const TArray<FString>& GetCurrentAttackCollisionInfos();
	void GetCurrentAttackInfos(float& OUT_Damage, bool& OUT_bIsDown, bool& OUT_bSetStunTimeToHitAnim, float& OUT_StunTime);

	// ex) ������ �ɷ��� �� ȣ��
	void ResetComboInfo();

protected:	
	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
	void UnBindEndDelegate();
	void BindEndDelegate();

protected:
	UPROPERTY()
	TObjectPtr<class ADK_Creature> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UDK_ComboActionData>> ComboActionDatas;

	// �� �ε����� ���� �޺��� �����
	uint8 CurComboActionDataIndex = 0;
	
	int32 CurrentCombo = 0;
	bool bHasNextComboCommand = false;
	bool bIsCallEnd = false;

	int8 ReserveComboActionDataIndex = -1;

		
};
