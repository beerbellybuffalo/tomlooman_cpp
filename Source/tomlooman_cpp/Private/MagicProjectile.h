// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "MagicProjectile.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API AMagicProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMagicProjectile();

protected:

	UPROPERTY(VisibleAnywhere)
	USphereComponent *SphereComp; // for collisions
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent *MovementComp; // for applying velocity and physics

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent *EffectComp; // for visual effects to be seen in the world

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
