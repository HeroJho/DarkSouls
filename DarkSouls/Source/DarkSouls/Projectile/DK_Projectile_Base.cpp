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
}

// Called when the game starts or when spawned
void ADK_Projectile_Base::BeginPlay()
{
	Super::BeginPlay();


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

void ADK_Projectile_Base::Init(AActor* Target, float InHomingAcceleration, float InSpeed, float InGravity, bool InbRotateToTarget, bool InbSimulating)
{
	if (InbSimulating == false)
		ProjectileMovementComponent->bSimulationEnabled = false;

	TargetActor = Target;
	HomingAcceleration = InHomingAcceleration;
	Speed = InSpeed;
	Gravity = InGravity;

	ProjectileMovementComponent->bRotationFollowsVelocity = InbRotateToTarget;
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
	
}

void ADK_Projectile_Base::SimulatingProjectile()
{
	ProjectileMovementComponent->bSimulationEnabled = true;
	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Cyan, FString::Printf(TEXT("Go")));
}




void ADK_Projectile_Base::RotateToTarget()
{
	// bRotationFollowsVelocity Option is true;

	FVector UnitVector = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), TargetActor->GetActorLocation());
	UnitVector *= Speed;
	
	ProjectileMovementComponent->Velocity = UnitVector;
}

void ADK_Projectile_Base::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == GetOwner())
		return;

	// 바인딩한 Hit 함수
	if(OnProjectileImpact.IsBound())
		OnProjectileImpact.Execute(OtherActor, Hit);

	if (IsValid(ImpactEffect))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location);
	}
		
	

	Destroy();
}
