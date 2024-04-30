// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
// #include "GameFramework/SpringArmComponent.h"
// #include "Camera/CameraComponent.h"	//bad for compilation time to include individually
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TOMLOOMAN_CPP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor>ProjectileClass;
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrowComp;
	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* MyCharacterMovement;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;
		
	UPROPERTY(EditAnywhere)
	float JumpMultiplier;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveSideways(float Value);
	void PrimaryAttack();
	void Jump();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
