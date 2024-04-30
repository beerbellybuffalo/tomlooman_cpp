// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInteractionComponent.h"

#include "MyGameplayInterface.h"
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
	FHitResult Hit;
	FVector EyeLocation;
	FRotator EyeRotation;
	FVector End = EyeLocation + (EyeRotation.Vector()*1000);
	MyOwner->GetActorEyesViewPoint(EyeLocation,EyeRotation);
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic); // must set chest object type accordingly

	GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		if (HitActor->Implements<UMyGameplayInterface>())
		{
			IMyGameplayInterface::Execute_Interact(HitActor,Cast<APawn>(MyOwner));
		}
	}
}

