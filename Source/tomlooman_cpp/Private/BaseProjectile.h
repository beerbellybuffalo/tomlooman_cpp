// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseProjectile.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API ABaseProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent *SphereComp; // for collisions
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent *MovementComp; // for applying velocity and physics

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UParticleSystemComponent *EffectComp; // for visual effects to be seen in the world

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxDistanceTillDetonate;

	FVector SpawnLocation;
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void Detonate();
};
