// Copyright Epic Games, Inc. All Rights Reserved.

#include "HaloRecreatedPickUpComponent.h"

UHaloRecreatedPickUpComponent::UHaloRecreatedPickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UHaloRecreatedPickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UHaloRecreatedPickUpComponent::OnSphereBeginOverlap);
}

void UHaloRecreatedPickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	AHaloRecreatedCharacter* Character = Cast<AHaloRecreatedCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Notify that the actor is being picked up
		OnPickUp.Broadcast(Character);

		// Unregister from the Overlap Event so it is no longer triggered
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
