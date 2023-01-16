// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GameFramework/Actor.h"

#include "TYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANGYUAN_API ATYPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
	ATYPlayerController();

private:
	AActor* SelectedActor;
	AActor* HoverActor;
};
