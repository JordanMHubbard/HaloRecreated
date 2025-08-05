// Copyright Epic Games, Inc. All Rights Reserved.

#include "HaloRecreatedGameMode.h"
#include "HaloRecreatedCharacter.h"
#include "UObject/ConstructorHelpers.h"

AHaloRecreatedGameMode::AHaloRecreatedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
