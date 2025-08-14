// Fill out your copyright notice in the Description page of Project Settings.

#include "MasterChief.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "TimerManager.h"


AMasterChief::AMasterChief()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	MasterChiefMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MasterChiefMesh"));
	MasterChiefMesh->SetOnlyOwnerSee(true);
	MasterChiefMesh->SetupAttachment(GetCapsuleComponent());
	MasterChiefMesh->SetCollisionProfileName(FName("NoCollision"));
	MasterChiefMesh->bCastDynamicShadow = false;
	MasterChiefMesh->CastShadow = false;
	MasterChiefMesh->SetRelativeLocation(FVector(0.f, 0.f, -98.f));
	
	// Create a CameraComponent	
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(MasterChiefMesh, FName("head"));
	FirstPersonCamera->SetRelativeLocationAndRotation(FirstPersonCameraOffset, FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;
}

// Input

void AMasterChief::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMasterChief::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMasterChief::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMasterChief::Look);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &AMasterChief::Shoot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AMasterChief::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMasterChief::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * MouseSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * MouseSensitivity);
	}
}

void AMasterChief::Shoot()
{
	if (isOnCooldown) return;

	isOnCooldown = true;
	BRBurst();
	GetWorldTimerManager().SetTimer(BRBurstTimerHandle, this, &AMasterChief::BRBurst, 0.138f, true);
}

void AMasterChief::BRBurst()
{
	FVector Start = FirstPersonCamera->GetComponentLocation();
	FVector End = Start + (FirstPersonCamera->GetForwardVector() * 3296.f);

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	UE_LOG(LogTemp, Warning, TEXT("Shot gun"));
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f, 0, 1.0f);
	if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
	{
		GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Blue, Hit.GetActor()->GetName());
	}
	
	if (++CurrentBulletsShot == MaxBulletsPerBurst)
	{
		CurrentBulletsShot = 0;
		GetWorldTimerManager().ClearTimer(BRBurstTimerHandle);
		BRBurstTimerHandle.Invalidate();
		GetWorldTimerManager().SetTimer(BRBurstCooldownHandle, this, &AMasterChief::BREndCooldown, 0.2f, false);
		UE_LOG(LogTemp, Warning, TEXT("done"));
	}
	
}

void AMasterChief::BREndCooldown()
{
	isOnCooldown = false;
}

