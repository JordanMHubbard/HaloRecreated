// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleRifleProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UMaterialInterface;

UCLASS()
class HALORECREATED_API ABattleRifleProjectile : public AActor
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* BulletHole;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	float BulletHoleSizeMin = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	float BulletHoleSizeMax = 7.f;

	bool IsDecalDisabled;

	FVector GetRandomBulletHoleSize()
	{
		float size = FMath::FRandRange(BulletHoleSizeMin, BulletHoleSizeMax);
		return FVector(size, size, size);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VFX, meta = (AllowPrivateAccess = "true"))
	float BulletHoleLifespan = 2.f;

public:
	ABattleRifleProjectile();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SetIsDecalDisabled( bool IsDisabled) { IsDecalDisabled = IsDisabled; } 
};
