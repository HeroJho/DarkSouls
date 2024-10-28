// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DK_ComboComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNoParmDelegate);

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
	void ProcessComboCommand(bool InbIsAllProcess);

	UFUNCTION(BlueprintCallable)
	void ChangeComboActionData(uint8 DataIndex);

	const TArray<FString>& GetCurrentAttackCollisionInfos();
	FS_DamageInfo GetCurrentAttackInfos();

	// ex) ������ �ɷ��� �� ȣ��
	void ResetComboInfo();
	
	// ���� Section ���� �Լ�
	void PlayNextSection();

	void ClearDelegate();
	
	UAnimMontage* GetCurMontage();
	

protected:	
	void ComboActionBegin();
	void BindEventFunc();
	void ComboCheck_Notify();

	UFUNCTION()
	void InterruptedComboAction(FName NotifyName);
	UFUNCTION()
	void EndComboAction(FName NotifyName);
	UFUNCTION()
	void BeginNotifyComboAction(FName NotifyName);
	UFUNCTION()
	void EndNotifyComboAction(FName NotifyName);


protected:
	UPROPERTY()
	TObjectPtr<class ADK_Object> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UDK_ComboActionData>> ComboActionDatas;

	// �� �ε����� ���� �޺��� �����
	uint8 CurComboActionDataIndex = 0;
	
	int32 CurrentCombo = 0;
	bool bHasNextComboCommand = false;

	int8 ReserveComboActionDataIndex = -1;

	bool bIsAllProcess = false;


	// Delegate
public:
	FOnNoParmDelegate OnSectionEndDelegate;
	FOnNoParmDelegate OnComboInterruptedDelegate;
	FOnNoParmDelegate OnComboEndDelegate;

	FOnNoParmDelegate OnComboInterruptedForTaskDelegate;
	FOnNoParmDelegate OnComboEndForTaskDelegate;

};
