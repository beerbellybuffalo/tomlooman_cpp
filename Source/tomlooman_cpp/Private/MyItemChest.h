// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "MyItemChest.generated.h"

UCLASS()
class TOMLOOMAN_CPP_API AMyItemChest : public AActor, public IMyGameplayInterface
{
	GENERATED_BODY()

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* LidMesh;
	UPROPERTY(EditAnywhere)
	float ChestLidRotation;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	AMyItemChest();
};
