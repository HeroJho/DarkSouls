// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DK_AnimObject.generated.h"

/**
 * 
 */
UCLASS()
class DARKSOULS_API UDK_AnimObject : public UAnimInstance
{
	GENERATED_BODY()

public:
	UDK_AnimObject();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Creature)
	TObjectPtr<class ADK_Object> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Creature)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsJumping : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsStun : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsKnockDown : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	uint8 bIsBlock : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Creature)
	float BlockSpeed;




};
