// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/DK_DamageableInterface.h"
#include "DK_Object.generated.h"



UCLASS()
class DARKSOULS_API ADK_Object : public ACharacter, public IDK_DamageableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADK_Object();

protected:
	virtual void PostInitializeComponents() override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	// Common
public:
	void AddImpulse(FVector Dir, float Powar);

	FORCEINLINE float GetBlockSpeed() { return BlockSpeed; }

	FORCEINLINE class UPlayMontageCallbackProxy* GetMontageCallbackProxy() { return PlayMontageCallbackProxy; }
	void SetMontageCallbackProxyWithIntrrupted(class UPlayMontageCallbackProxy* Proxy);


protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float NormalSpeed = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
	float BlockSpeed = 150.f;

	UPROPERTY()
	TObjectPtr<class UPlayMontageCallbackProxy> PlayMontageCallbackProxy;



	// UI
public:
	void OnOffScreenHPBar(bool bIsOn);
	void OnOffHUDHPBar(bool bIsOn);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = UI)
	TObjectPtr<class UDK_WidgetComponent> WidgetComponent;

	UPROPERTY()
	TWeakObjectPtr<class UDK_SmoothBarWidget> BossSmoothBarWidget;

	FDelegateHandle BossHpBarDelegateHandle;

	//UPROPERTY(EditAnywhere, Category = BossHpBar)
	//TSubclassOf<class UDK_SmoothBarWidget> BossHpBarWidgetClass;



	// Stat
protected:
	virtual void OnHitReaction_Notify(EDamageResponse DamageResponseType, int32 GPValue, float StunTime, float KnockBackPowar, AActor* DamageCauser);
	virtual void OnBlock_Notify(bool bCanParrying, float KnockBackPowar, AActor* DamageCauser);
	virtual void OnDodgeSkip_Notify();

protected:
	TWeakObjectPtr<class UDK_StatComponent> StatComponent;



	// Combo && Attack
public:
	FORCEINLINE class UDK_ComboComponent* GetComboComponent() { return ComboComponent; }
	FORCEINLINE bool IsInAttackRange() { return bIsInAttackRange; }
	FORCEINLINE bool IsAttacking() { return bIsAttacking; }

	void BeginAttackRange_Notify();
	void EndAttackRange_Notify();

	void BeginColRange_Notify();
	void EndColRange_Notify();

	virtual void InterruptedAttack_Notify();

	void EndAttack_Notify();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Combo)
	TObjectPtr<class UDK_ComboComponent> ComboComponent;

	bool bIsAttacking = false;
	bool bIsInAttackRange = false;







	// Collsion
public:
	virtual void OnColHit_Notify(IDK_DamageableInterface* HitActor);

protected:
	virtual void DamagedByGPAttacked(int32 GPValue);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Collision)
	TObjectPtr<class UDK_CollisionManagerComponent> CollisionManagerComponent;






	// Stun
public:
	FORCEINLINE bool IsStun() { return bIsStun; }
	FORCEINLINE bool IsKnockDown() { return bIsKnockDown; }
	virtual void SetIsKnockDown(bool bValue);

	FORCEINLINE bool GetSmallHittedTrigger() { return bSmallHittedTrigger; }


public:	
	virtual void Stun(float StunTime, bool bSetAnimTime = false);
	virtual void KnockDown(float KnockDownTime);
	virtual void SmallHittedTrigger();

protected:
	virtual void EndStun();
	void StartEndKnockDown();
	UFUNCTION()
	void EndKnockDown(FName NotifyName);

protected:
	FTimerHandle StunTimerHandle;
	bool bIsStun = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> HitMontage;


	FTimerHandle KnockDownTimerHandle;
	FTimerHandle EndKnockDownTimerHandle;
	bool bIsKnockDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init)
	TObjectPtr<class UAnimMontage> StartKnockDownMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Init)
	TObjectPtr<class UAnimMontage> EndKnockDownMontage;


	FTimerHandle SmallHittedTimerHandle;
	bool bSmallHittedTrigger = false;



	// SmoothTurn
public:
	UFUNCTION(BlueprintCallable)
	void SmoothTurnByCallOnce(FVector InDestPos, float TurnSpeed);
	void StopSmoothTurn();

protected:
	void SmoothTurnTick();

protected:
	bool bIsSmoothTurn = false;
	float SmoothTurnSpeed = 0.f;
	FVector DestPos;






	// Dodge
protected:
	virtual void Dodge();

	UFUNCTION()
	void EndDoge(FName NotifyName);
	UFUNCTION()
	void BeginNotifyDoge(FName NotifyName);
	UFUNCTION()
	void EndNotifyDoge(FName NotifyName);

	virtual void BeginDodgeSkip_Notify();
	virtual void EndDodgeSkip_Notify();

protected:
	bool bIsDodge = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Dodge, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DodgeMontage;






	// Block
public:
	bool IsBlock();


	virtual void Block();
	virtual void EndBlock();

	void HitBlock();
	void EndHitBlock();

	// 퍼펙트 가드 당했을 때 호출되는 함수
	virtual void BeBlockedPerfectly(int32 GPValue);


protected:
	virtual void BlockAttack(bool bCanParrying, float KnockBackPowar, AActor* DamageCauser);




protected:
	bool bIsHitBlock = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Block, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> BlockHitMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Block, Meta = (AllowPrivateAccess = "true"))
	float HitTime = 0.f;
	
	FTimerHandle HitBlockTimerHandle;




	// MiddlePos
	// 오브젝트 Location을 가져오면 발바닥 위치를 가져옴.
	// 내가 원하는(오브젝트 크기마다 다름) 가슴 부분을 가져오고 싶다.
public:
	FVector GetMiddlePos();

protected:
	UPROPERTY(EditAnywhere, Category = MiddlePos)
	TObjectPtr<class UArrowComponent> MiddlePosArrowComponent;




	// Condition
protected:
	virtual bool CanAttack();
	virtual bool CanDodge();
	virtual bool CanSmoothTurn();
	virtual bool CanStun();
	virtual bool CanKnockDown();
	virtual bool CanBlock();


	// 상태에 들어가면 초기화해야할 작업들
	// 공격 시,
	virtual void ResetInfoOnAttack();
	// 스턴 시, 
	virtual void ResetInfoOnStun();
	// 기절 시,
	virtual void ResetInfoOnKnockDown();
	// 회피 시,
	virtual void ResetInfoOnDodge();
	// 가드 시,
	virtual void ResetInfoOnBlock();
	// 가드 힛 시,
	virtual void ResetInfoOnHitBlock();







	// ======  Inteface ======
	// Damageable
public:
	virtual bool TakeDamage(FS_DamageInfo DamageInfo, AActor* DamageCauser) override;
	
	virtual bool IsDead();

};
