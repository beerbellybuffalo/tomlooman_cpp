// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent *CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *MeshComp;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *EffectComp;
	UPROPERTY(EditAnywhere)
	float ExplosionRadius;

	// void Explode(UParticleSystem* ExplosionEffect, FHitResult Hit);
	void Explode();

	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	// UFUNCTION()
	// void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, FHitResult& Hit);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ApplyExplosionForceInRadius(FVector StartLocation, FVector EndLocation, float Radius);
};
