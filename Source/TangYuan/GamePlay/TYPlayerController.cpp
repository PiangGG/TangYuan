// Fill out your copyright notice in the Description page of Project Settings.


#include "TYPlayerController.h"


ATYPlayerController::ATYPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
}

void ATYPlayerController::SetControllerState(EControllerState State)
{
	ControllerState=State;

	switch (ControllerState)
	{
		case EControllerState::EBuild:
			
			break;
		
		case EControllerState::ECombat:
			
			break;
		default: ;
	}
}

EControllerState ATYPlayerController::GetControllerState()
{
	return ControllerState;
}
