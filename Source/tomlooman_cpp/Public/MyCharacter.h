// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyInteractionComponent.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"
// #include "GameFramework/SpringArmComponent.h"
// #include "Camera/CameraComponent.h"	//bad for compilation time to include individually

// Add Forward Declarations here
class UCameraComponent;
class USpringArmComponent;
class UMyInteractionComponent;
class UAnimMontage;

UCLASS()
class TOMLOOMAN_CPP_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor>PrimaryProjectile;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor>BlackholeProjectile;
	UPROPERTY(EditAnywhere, Category = "Projectiles")
	TSubclassOf<AActor>TeleportProjectile;
	UPROPERTY(VisibleAnywhere, Category = "Projectiles")
	TSubclassOf<AActor>ActiveProjectile;

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
	UPROPERTY(VisibleAnywhere)
	UMyInteractionComponent* InteractionComp;
	UPROPERTY(EditAnywhere)
	float JumpMultiplier;
	UPROPERTY(EditAnywhere, Category = "Attack") // Category here is for the sake of making things easier for us in the Editor
	UAnimMontage* AttackAnim;
	FTimerHandle TimerHandle_Attack;

	void MoveForward(float Value);
	void MoveSideways(float Value);
	void Attack();
	void Attack_TimeElapsed(); // this is a temporary way of adding delay to sync the animation with the projectile. proper way is to use ANIMATION NOTIFIES
	void PrimaryInteract();
	void Jump() override; //because the default ACharacter class in Unreal Engine already has an implementation for Jump(). The function in ACharacter is marked with the "virtual" specifier and hence can be overriden.
	FVector GetAttackImpactPoint();
	FVector ImpactPoint;
	FRotator ProjectileRot;
	void SetAndFirePrimaryProjectile();
	void SetAndFireBlackholeProjectile();
	void SetAndFireTeleportProjectile();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Sets default values for this character's properties
	AMyCharacter();
};
