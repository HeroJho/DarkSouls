// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tool/Struct.h"
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
	FAttackDamagedInfo GetCurrentAttackInfos();

	// ex) 스턴이 걸렸을 때 호출
	void ResetComboInfo();

protected:	
	void ComboActionBegin();
	
	void BindEventFunc();
	
	UFUNCTION()
	void ComboActionInterrupted(FName NotifyName);
	UFUNCTION()
	void ComboActionEnd(FName NotifyName);



protected:
	UPROPERTY()
	TObjectPtr<class ADK_Creature> Owner;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<class UDK_ComboActionData>> ComboActionDatas;

	// 이 인덱스에 따라 콤보가 실행됨
	uint8 CurComboActionDataIndex = 0;
	
	int32 CurrentCombo = 0;
	bool bHasNextComboCommand = false;

	int8 ReserveComboActionDataIndex = -1;

};
