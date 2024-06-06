// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseProjectile.h"

#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABaseProjectile::ABaseProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	RootComponent = SphereComp;
	SphereComp->SetGenerateOverlapEvents(true);
	
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	MaxDistanceTillDetonate = 800.0f;
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	//save spawn location
	SpawnLocation = GetActorLocation();
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//check if projectile has moved more than the threshold distance from the spawn point. If true, then call detonate() function and complete implementation in BP.
	if (FVector::Distance(SpawnLocation,GetActorLocation()) >= MaxDistanceTillDetonate)
	{
		Detonate();
	}
}

void ABaseProjectile::Detonate()
{
	// Make sure this function is called on a valid instance of a Blueprint
	if (UFunction* CustomEvent = FindFunction(TEXT("DetonateEvent")))
	{
		// No parameters to pass, so we use an empty struct
		struct FCustomEventParams
		{
		};

		FCustomEventParams Params;
		ProcessEvent(CustomEvent, &Params);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Detonate event not found! Make sure the Blueprint implements DetonateEvent."));
	}
}

