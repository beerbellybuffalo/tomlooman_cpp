// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"

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
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AExplosiveBarrel::Explode()
{
	// get the effect that's assigned via editor
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
		//Destroy both the barrel and the projectile
		OverlappedActor->Destroy();
		OtherActor->Destroy();
		//trigger explosion effect
		Explode();
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