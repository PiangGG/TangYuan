// Fill out your copyright notice in the Description page of Project Settings.


#include "TYGameMode.h"

#include "TYCharacter.h"
#include "TYPlayerController.h"
#include "GameFramework/DefaultPawn.h"

ATYGameMode::ATYGameMode()
{
	//DefaultPawnClass = ATYCharacter::StaticClass();
	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = ATYPlayerController::StaticClass();
}
