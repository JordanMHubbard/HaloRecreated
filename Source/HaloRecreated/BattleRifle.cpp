// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleRifle.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MasterChief.h"
#include "BattleRifleProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void UBattleRifle::BeginPlay()
{
	Super::BeginPlay();

	PlayerCharacter = Cast<AMasterChief>(GetOwner());
	PlayerCamera = PlayerCharacter->GetFirstPersonCamera();

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(PlayerCharacter->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			// Fire
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UBattleRifle::Fire);
		}
	}
}

void UBattleRifle::Fire()
{
	if (IsOnCooldown) return;

	IsOnCooldown = true;
	Burst();
	GetWorld()->GetTimerManager().SetTimer(BRBurstTimerHandle, this, &UBattleRifle::Burst, 0.138f, true);
}

void UBattleRifle::Burst()
{
	if (PlayerCharacter == nullptr || PlayerCamera == nullptr)
	{
		return;
	}
	
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = PlayerCamera->GetComponentRotation();
			const FVector SpawnLocation = PlayerCamera->GetComponentLocation() + PlayerCamera->GetForwardVector() * 75.f;;
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			World->SpawnActor<ABattleRifleProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			UE_LOG(LogTemp, Warning, TEXT("Shot BR"));

			if (++CurrentBulletsShot == MaxBulletsPerBurst)
			{
				CurrentBulletsShot = 0;
				GetWorld()->GetTimerManager().ClearTimer(BRBurstTimerHandle);
				BRBurstTimerHandle.Invalidate();
				GetWorld()->GetTimerManager().SetTimer(BRBurstCooldownHandle, this, &UBattleRifle::EndCooldown, 0.2f, false);
				UE_LOG(LogTemp, Warning, TEXT("done"));
			}
		}
	}

	// Try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, PlayerCharacter->GetActorLocation());
	}
	
	// Try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = PlayerCharacter->GetMesh1P()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void UBattleRifle::EndCooldown()
{
	IsOnCooldown = false;
}


