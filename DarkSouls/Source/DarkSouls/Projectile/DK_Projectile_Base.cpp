// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/DK_Projectile_Base.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Creature/DK_Object.h"


// Sets default values
ADK_Projectile_Base::ADK_Projectile_Base()
{
 	PrimaryActorTick.bCanEverTick = false;

	// 생성자에서 생성 후 Hit 함수 바인딩
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	FScriptDelegate HitDelegate;
	HitDelegate.BindUFunction(this, FName("OnComponentHit"));
	BoxCollision->OnComponentHit.AddUnique(HitDelegate);

	RootComponent = BoxCollision;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(BoxCollision);


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	Speed = 0.f;
	Gravity = 0.f;
	HomingAcceleration = 0.f;
	bSimulating = false;
	RotSpeed = FRotator::ZeroRotator;
}

// Called when the game starts or when spawned
void ADK_Projectile_Base::BeginPlay()
{
	Super::BeginPlay();

	RotateToTarget();
	BoxCollision->IgnoreActorWhenMoving(Owner, true);
}

// Called every frame
void ADK_Projectile_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (ProjectileMovementComponent->bSimulationEnabled == false)
		return;

	if (RotSpeed != FRotator::ZeroRotator)
	{
		AddActorWorldRotation(RotSpeed * DeltaTime);
	}

}

void ADK_Projectile_Base::Init(ProjectileOption Option)
{
	if (Option.bSimulating == false)
		ProjectileMovementComponent->bSimulationEnabled = false;

	TargetActor = Option.Target;
	HomingAcceleration = Option.HomingAcceleration;
	Speed = Option.Speed;
	Gravity = Option.Gravity;

	ProjectileMovementComponent->bRotationFollowsVelocity = Option.bRotateToTarget;
	ProjectileMovementComponent->InitialSpeed = Speed;
	ProjectileMovementComponent->MaxSpeed = Speed;
	ProjectileMovementComponent->ProjectileGravityScale = Gravity;

	ProjectileMovementComponent->bIsHomingProjectile = false;
	if (TargetActor.IsValid() && HomingAcceleration > 0.f)
	{
		ProjectileMovementComponent->bIsHomingProjectile = true;
		DisableComponentsSimulatePhysics();
		ProjectileMovementComponent->HomingTargetComponent = TargetActor.Get()->GetRootComponent();
		ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAcceleration;
	}

	if (RotSpeed != FRotator::ZeroRotator)
	{
		PrimaryActorTick.bCanEverTick = true;
		ProjectileMovementComponent->bRotationFollowsVelocity = false;
	}
	

	if (UKismetMathLibrary::NearlyEqual_FloatFloat(LifeTime, 0.f) == false)
	{
		FTimerDelegate Del;
		Del.BindUFunction(this, FName("DestroyProjectile"), FVector::ZeroVector);
		GetWorldTimerManager().SetTimer(LifeTimerHandle, Del, LifeTime, false);
	}


}

void ADK_Projectile_Base::SimulatingProjectile()
{
	ProjectileMovementComponent->bSimulationEnabled = true;

	RotateToTarget();
}




void ADK_Projectile_Base::RotateToTarget()
{
	// bRotationFollowsVelocity Option is true;
	if (TargetActor.IsValid() == false)
		return;

	FVector TargetLocation = TargetActor->GetActorLocation();

	ADK_Object* ObjTarget = Cast<ADK_Object>(TargetActor);
	if (IsValid(ObjTarget))
	{
		TargetLocation = ObjTarget->GetMiddlePos();
	}

	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetLocation);
	UnitVector *= Speed;
	
	ProjectileMovementComponent->Velocity = UnitVector;
}

void ADK_Projectile_Base::DestroyProjectile(FVector HitPos)
{
	if (IsValid(ImpactEffect))
	{
		if(HitPos == FVector::ZeroVector)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, GetActorLocation());
		else
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitPos);
	}

	Destroy();
}

void ADK_Projectile_Base::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!ProjectileMovementComponent->bSimulationEnabled)
		return;

	// 바인딩한 Hit 함수
	if(OnProjectileImpact.IsBound())
		OnProjectileImpact.Execute(OtherActor);
		
	DestroyProjectile(Hit.ImpactPoint);

}
