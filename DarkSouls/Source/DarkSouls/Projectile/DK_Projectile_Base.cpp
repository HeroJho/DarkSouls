// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DK_Projectile_Base.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ADK_Projectile_Base::ADK_Projectile_Base()
{
 	PrimaryActorTick.bCanEverTick = false;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	FScriptDelegate BeginDelegate;
	BeginDelegate.BindUFunction(this, FName("OnComponentHit"));
	BoxCollision->OnComponentHit.AddUnique(BeginDelegate);

	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(BoxCollision);



	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	
	Speed = 0.f;
	Gravity = 0.f;
	bIsHoming = false;
}

// Called when the game starts or when spawned
void ADK_Projectile_Base::BeginPlay()
{
	Super::BeginPlay();

	// 오너랑 부딪히지 않는다, 오너쪽에도 호출해줘야 함
	BoxCollision->IgnoreActorWhenMoving(GetOwner(), true);

	if (TargetActor.IsValid())
	{
		RotateToTarget();
	}


}

// Called every frame
void ADK_Projectile_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADK_Projectile_Base::RotateToTarget()
{
	// bRotationFollowsVelocity Option is true;

	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetActor->GetActorLocation());
	UnitVector *= Speed;
	
	ProjectileMovementComponent->Velocity = UnitVector;

}

void ADK_Projectile_Base::OnComponentHit(UPrimitiveComponent* OnComponentHit, UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// OnProjectileImpact.Execute(OtherActor, Hit);

	if(IsValid(ImpactEffect))
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location);

	Destroy();
}

