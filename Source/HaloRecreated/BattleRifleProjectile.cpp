// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleRifleProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Materials/MaterialInterface.h"
#include "Kismet/GameplayStatics.h"

ABattleRifleProjectile::ABattleRifleProjectile()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(FName("BulletCollision"));
	CollisionComp->InitSphereRadius(2.f);
	CollisionComp->BodyInstance.SetCollisionProfileName("BulletProjectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABattleRifleProjectile::OnHit);

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 18000.f;
	ProjectileMovement->MaxSpeed = 18000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bShouldBounce = false;
}

void ABattleRifleProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, OtherActor->GetName());

		FRotator DecalRotation = Hit.ImpactNormal.Rotation();
		DecalRotation.Roll = FMath::FRandRange(0.f, 360.f);

		if (!IsDecalDisabled)
		{
			UGameplayStatics::SpawnDecalAtLocation(
				GetWorld(),
				BulletHole,
				GetRandomBulletHoleSize(),
				Hit.ImpactPoint + Hit.ImpactNormal * 1.f,
				DecalRotation,
				BulletHoleLifespan
			);
		}
		Destroy();
	}
}



