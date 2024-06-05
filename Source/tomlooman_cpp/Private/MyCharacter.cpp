// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// This is entirely optional, it draws two arrows to visualize rotations of the player
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);
}

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp -> bUsePawnControlRotation = true;
	SpringArmComp -> SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp -> SetupAttachment(SpringArmComp); //we defined this pointer to USpringArmComp inside the header

	MyCharacterMovement = GetCharacterMovement();
	MyCharacterMovement -> bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false; // we do this if we want the character to be able to face the player 

	JumpMultiplier = 500;

	RootComp = GetRootComponent();
	Cast<UPrimitiveComponent>(RootComp)->SetSimulatePhysics(true);

	InteractionComp = CreateDefaultSubobject<UMyInteractionComponent>("InteractionComp");
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// // Called every frame
// void AMyCharacter::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// }

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward",this,&AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveSideways",this,&AMyCharacter::MoveSideways);
	PlayerInputComponent->BindAxis("Turn",this,&APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp",this,&APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &AMyCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &AMyCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMyCharacter::Jump);
}

void AMyCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	AddMovementInput(ControlRot.Vector(),Value);
}

void AMyCharacter::MoveSideways(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X forward (red)
	//Y right (green)
	//Z up (blue)
	
	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	AddMovementInput(RightVector,Value);
}

void AMyCharacter::PrimaryAttack()
{
	float Delay = 0.2f;
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack,this,&AMyCharacter::PrimaryAttack_TimeElapsed,Delay);
}

FVector AMyCharacter::GetAttackImpactPoint()
{
	// Get the player controller
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// Get the viewport size
	int32 ViewportSizeX, ViewportSizeY;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	// Calculate the crosshair screen position
	FVector2D CrosshairPosition(ViewportSizeX * 0.5f, ViewportSizeY * 0.5f);

	// Deproject the screen position of the crosshair to a world position and direction
	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(CrosshairPosition.X, CrosshairPosition.Y, WorldLocation, WorldDirection);

	// Calculate the end point of the line trace
	FVector End = WorldLocation + (WorldDirection * 10000.0f);

	// Define the object types to trace against
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Destructible));
	// ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn)); //no enemy units yet
	// Add more object types as needed
	
	// Perform the line trace
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, WorldLocation, End, FCollisionObjectQueryParams(ObjectTypes), QueryParams);

	// Draw debug line
	DrawDebugLine(GetWorld(), WorldLocation, End, FColor::Green, false, 1, 0, 1);

	// Check if we hit something
	if (bHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *HitResult.GetActor()->GetName());
		DrawDebugPoint(GetWorld(), HitResult.Location, 10, FColor::Red, false, 1);
		return HitResult.ImpactPoint;
	}
	//if nothing was hit
	return HitResult.TraceEnd;
}

void AMyCharacter::PrimaryAttack_TimeElapsed()
{
	if(ensure(ProjectileClass))
	{
		//Step 1: set the projectile to spawn at muzzle
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");

		//Step 2: perform line trace from camera to world to find the impact point for the projectile.
		ImpactPoint = GetAttackImpactPoint();
		
		//Step 3: projectile will be set to travel from hand to impact point when spawned
		ProjectileRot = UKismetMathLibrary::FindLookAtRotation(HandLocation, ImpactPoint);
		FTransform SpawnTM = FTransform(ProjectileRot,HandLocation);
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}

void AMyCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}

void AMyCharacter::Jump()
{
	if(!MyCharacterMovement->IsFalling())
	{
		LaunchCharacter(GetActorUpVector()*JumpMultiplier,false,false);

		// MyCharacterMovement->AddImpulse(GetActorUpVector()*1000);
	}
}