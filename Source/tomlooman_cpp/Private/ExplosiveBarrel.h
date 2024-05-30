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

	// UPROPERTY(VisibleAnywhere)
	// UCapsuleComponent *CapsuleComp;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent *MeshComp;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *EffectComp;
	UPROPERTY(EditAnywhere)
	float ExplosionRadius;
	UPROPERTY(EditAnywhere)
	float ExplosionMagnitude;
	UPROPERTY(VisibleAnywhere)
	TArray<AActor*> ActorsToIgnore;
	

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// void Explode(UParticleSystem* ExplosionEffect, FHitResult Hit);

	UFUNCTION(BlueprintCallable)
	void Explode();

	// UFUNCTION()
	// void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	
	// UFUNCTION()
	// void OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, FHitResult& Hit);

	void ApplyExplosionForceInRadius(FVector StartLocation, FVector EndLocation, float Radius);
};
