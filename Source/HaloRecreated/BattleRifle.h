// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BattleRifle.generated.h"

class AMasterChief;
class UCameraComponent;
class UNiagaraSystem;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HALORECREATED_API UBattleRifle : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABattleRifleProjectile> ProjectileClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta=(AllowPrivateAccess = "true"))
	USoundBase* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta=(AllowPrivateAccess = "true"))
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay, meta=(AllowPrivateAccess = "true"))
	UNiagaraSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION()
	void Burst();

	UFUNCTION()
	void EndCooldown();

	FTimerHandle BRBurstTimerHandle;
	FTimerHandle BRBurstCooldownHandle;
	int32 CurrentBulletsShot = 0;
	int32 MaxBulletsPerBurst = 3;
	float FireRate = 0.06f;
	bool IsOnCooldown;

protected:
	virtual void BeginPlay() override;

private:
	/** The PlayerCharacter holding this weapon*/
	UPROPERTY()
	AMasterChief* PlayerCharacter;
	
	UPROPERTY()
	UCameraComponent* PlayerCamera;
};
