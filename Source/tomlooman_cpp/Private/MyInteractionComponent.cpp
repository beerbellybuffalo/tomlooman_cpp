// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractionComponent.h"

#include "MyCharacter.h"
#include "MyGameplayInterface.h"
#include "Camera/CameraComponent.h"
#include "EntitySystem/MovieSceneEntitySystemRunner.h"

// Sets default values for this component's properties
UMyInteractionComponent::UMyInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UMyInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMyInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UMyInteractionComponent::PrimaryInteract()
{
	AActor* MyOwner = GetOwner();
	FHitResult LineHit; // this is for line trace only, don't need this for sphere

	// // Version 1.1: using GetActorEyesViewPoint()
	// FVector EyeLocation;
	// FRotator EyeRotation;
	// MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	// FVector End = EyeLocation + (EyeRotation.Vector()*1000);

	// // Version 1.2: using location and rotation of the CameraComponent on the Character
	AMyCharacter* MyChar = Cast<AMyCharacter>(MyOwner);
	UCameraComponent* MyCam = MyChar->CameraComp;
	FVector CamLocation = MyCam->GetComponentLocation();
	FRotator CamRotation = MyCam->GetComponentRotation();
	FVector End = CamLocation + CamRotation.Vector()*1000;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // must set chest object type accordingly
	// ObjectQueryParams.RemoveObjectTypesToQuery(ECC_Pawn); //ignore player character

	// // Version 1.1: using GetActorEyesViewPoint()
	// GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	// // Version 1.2: using location and rotation of the CameraComponent on the Character
	bool bLineTraceHit = GetWorld()->LineTraceSingleByObjectType(LineHit, CamLocation, End, ObjectQueryParams);
	
	// **Note that line trace only works on objects that have collision enabled.

	// // Version 1.3: using a sphere trace
	TArray<FHitResult> Hits;
	FCollisionShape ShapeForTrace;
	float Radius = 100.0f;
	ShapeForTrace.SetSphere(Radius);
	bool bSphereTraceHit = GetWorld()->SweepMultiByObjectType(Hits,CamLocation,End,FQuat::Identity,ObjectQueryParams,ShapeForTrace);

	for (FHitResult Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			FString ActorName = HitActor->GetHumanReadableName();
			UE_LOG(LogTemp,Warning,TEXT("%s"), *ActorName);
			if (HitActor->Implements<UMyGameplayInterface>())
			{
				IMyGameplayInterface::Execute_Interact(HitActor,Cast<APawn>(MyOwner));
			}
			// // Version 1.3: using sphere
			// FColor LineColor = bSphereTraceHit? FColor::Green : FColor::Red;
			// DrawDebugSphere(GetWorld(),Hit.ImpactPoint,Radius,32,LineColor,false,2.0f,0,2.0f);
		}
	}

	// // Version 1.1: using GetActorEyesViewPoint()
	// DrawDebugLine(GetWorld(),EyeLocation,End,FColor::Red,false,2.0f,0,2.0f) ;
	
	FColor LineColor = bLineTraceHit? FColor::Green : FColor::Red;
	// // Version 1.2: using location and rotation of the CameraComponent on the Character
	DrawDebugLine(GetWorld(),CamLocation,End,LineColor,false,2.0f,0,2.0f) ;
		
}

