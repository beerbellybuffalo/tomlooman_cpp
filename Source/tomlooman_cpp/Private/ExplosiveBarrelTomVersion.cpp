// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrelTomVersion.h"

#include "PhysicsEngine/RadialForceComponent.h"


// Sets default values
AExplosiveBarrelTomVersion::AExplosiveBarrelTomVersion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ForceComp = CreateDefaultSubobject<URadialForceComponent>("ForceComp");
	ForceComp->SetupAttachment(MeshComp);

	//because by default if left true, a small constant force is applied. See URadialForceComponent
	ForceComp->SetAutoActivate(false);

	ForceComp->Radius = 750.0f;
	ForceComp->ImpulseStrength = 1500.0f; //this will be much higher like 200000 if bImpulseVelChange = false
	ForceComp->bImpulseVelChange = true; //mass of objects are ignored when applying impulse

	ForceComp->AddCollisionChannelToAffect(ECC_WorldDynamic);
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AExplosiveBarrelTomVersion::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	MeshComp->OnComponentHit.AddDynamic(this, &AExplosiveBarrelTomVersion::OnActorHit);
}

void AExplosiveBarrelTomVersion::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	ForceComp -> FireImpulse();
}

// Called when the game starts or when spawned
void AExplosiveBarrelTomVersion::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrelTomVersion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

