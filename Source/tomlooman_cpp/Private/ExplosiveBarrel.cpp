// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CollisionQueryParams.h"
#include "LandscapeStreamingProxy.h"
#include "MagicProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	// Bind the OnActorBeginOverlap event to your function
	OnActorBeginOverlap.AddDynamic(this, &AExplosiveBarrel::OnOverlapBegin);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComp");
	RootComponent = CapsuleComp;
	CapsuleComp->SetGenerateOverlapEvents(true);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(RootComponent);
	ExplosionRadius = 500; //set some default value but can change in editor
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	// when spawned, set all objects of the class ALandscapeProxy to be ignored.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALandscapeStreamingProxy::StaticClass(), ActorsToIgnore);
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosiveBarrel::ApplyExplosionForceInRadius(FVector StartLocation, FVector EndLocation, float Radius)
{
	// // Ignore WorldStatic objects for all traces
	// ETraceTypeQuery TraceTypeQuery = UEngineTypes::ConvertToTraceType(ECC_WorldDynamic);

	// Out parameters for hit results
	TArray<FHitResult> OutHits;

	// Trace parameters
	FCollisionQueryParams TraceParams(FName(TEXT("MultiSphereTrace")), false, this);
	UE_LOG(LogTemp, Warning, TEXT("Trace parameters successful"));
	
	// Perform the multi-sphere trace
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		Radius,
		UEngineTypes::ConvertToTraceType(ECC_WorldDynamic),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		OutHits,
		true);
	UE_LOG(LogTemp, Warning, TEXT("Multi-sphere trace successful"));

	// Check if the trace hit anything
	if (bHit)
	{
		// Loop through each hit result
		for (const FHitResult &Hit : OutHits)
		{
			UE_LOG(LogTemp, Warning, TEXT("Entered For Loop"));
			
			// Access the hit actor and other relevant information
			AActor* HitActor = Hit.GetActor();
			FString ActorName = HitActor->GetName();
			UE_LOG(LogTemp, Warning, TEXT("Actor hit: %s"), *ActorName);
			USceneComponent* RootComp = HitActor->GetRootComponent();
			FVector HitLocation = Hit.ImpactPoint;
			FVector ImpulseDirection = HitLocation-StartLocation;
			ImpulseDirection.Normalize();
			
			//this multiplier is inversely proportional to the distance from the origin of explosion
			float forceMultiplierByDist = ExplosionRadius/(HitLocation-StartLocation).Size();
			// Apply Impulse on HitActor towards ImpulseDirection, using forceMultiplierByDist
			// // If the actor has a root component, apply impulse
			if (RootComp)
			{
				UPrimitiveComponent* RootPrimComp = Cast<UPrimitiveComponent>(RootComp);
				RootPrimComp->AddImpulse(forceMultiplierByDist*ImpulseDirection);
				UE_LOG(LogTemp, Warning, TEXT("Impulse Added Successfully"));
			}
		}
	}
}

void AExplosiveBarrel::Explode()
{
	// get the effect that's assigned via editor and spawn it
	UParticleSystem* ExplosionEffect = EffectComp->Template;
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation());
}

//Method 1: Using Overlap to trigger barrel explosion
void AExplosiveBarrel::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Barrel Hit, Overlap Detected"));
	// Check if the other actor in collision is of the class AMagicProjectile
	if(OtherActor->IsA(AMagicProjectile::StaticClass()))
	{
		OtherActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Projectile Destroyed"));
		FVector ExplosionLocation = OverlappedActor->GetActorLocation();
		//trigger explosion effect
		Explode();
		UE_LOG(LogTemp, Warning, TEXT("Explode() finished execution"));
		
		// apply explosion force to surrounding objects within a certain radius
		ApplyExplosionForceInRadius(ExplosionLocation, ExplosionLocation, ExplosionRadius);
		UE_LOG(LogTemp, Warning, TEXT("ApplyExplosionForceInRadius() finished execution"));

		//Destroy both the barrel and the projectile
		OverlappedActor->Destroy();
		UE_LOG(LogTemp, Warning, TEXT("Barrel Destroyed"));
	}
}


//Method 2: Implement explosion using OnHit instead. For some reason, the OnHit Event was not being detected.
// void AExplosiveBarrel::Explode(UParticleSystem* ExplosionEffect, FHitResult Hit)
// {
// 	// Destroy Barrel
// 	Destroy();
// 	// Spawn and attach the explosion effect to this barrel
// 	UParticleSystemComponent* ParticleComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, Hit.ImpactPoint, FRotator::ZeroRotator);
// 	ParticleComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
// }
//
// void AExplosiveBarrel::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, FHitResult& Hit)
// {
// 	UE_LOG(LogTemp, Warning, TEXT("Barrel Hit Detected"));
// 	// Check if the other actor in collision is of the class AMagicProjectile
// 	if(OtherActor->IsA(AMagicProjectile::StaticClass()))
// 	{
// 		UParticleSystem* ExplosionEffect = EffectComp->Template;
// 		Explode(ExplosionEffect,Hit);
// 	}
// }