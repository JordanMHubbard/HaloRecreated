// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "BattleRifle.generated.h"

class AMasterChief;
class UCameraComponent;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HALORECREATED_API UBattleRifle : public USkeletalMeshComponent
{
	GENERATED_BODY()
	
public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ABattleRifleProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Make the weapon Fire a Projectile */
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
