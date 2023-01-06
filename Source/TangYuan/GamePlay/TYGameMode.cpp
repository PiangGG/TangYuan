// Fill out your copyright notice in the Description page of Project Settings.


#include "TYGameMode.h"

#include "TYCharacter.h"
#include "TYPlayerController.h"

ATYGameMode::ATYGameMode()
{
	DefaultPawnClass = ATYCharacter::StaticClass();
	PlayerControllerClass = ATYPlayerController::StaticClass();
}
