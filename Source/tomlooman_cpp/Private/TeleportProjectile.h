// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "TeleportProjectile.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API ATeleportProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATeleportProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
