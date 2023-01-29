// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GameFramework/Actor.h"
#include "TangYuan/Tool/EnumLib.h"

#include "TYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANGYUAN_API ATYPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
	ATYPlayerController();
public:
	
	
private:
	AActor* SelectedActor;
	
	AActor* HoverActor;
public:
	//控制器状态
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="")
	EControllerState ControllerState = EControllerState::ECombat;
	
	UFUNCTION(BlueprintCallable)
	void SetControllerState(EControllerState State);

	UFUNCTION(BlueprintCallable)
	EControllerState GetControllerState();
};
