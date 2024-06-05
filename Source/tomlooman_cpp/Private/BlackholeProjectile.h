// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "BlackholeProjectile.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API ABlackholeProjectile : public ABaseProjectile
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABlackholeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
