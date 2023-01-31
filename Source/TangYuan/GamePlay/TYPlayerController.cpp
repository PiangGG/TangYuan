// Fill out your copyright notice in the Description page of Project Settings.


#include "TYPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "TangYuan/Attacker/BaseAttacker.h"
#include "TangYuan/Tool/ToolLibrary.h"


ATYPlayerController::ATYPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
}

void ATYPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UpdateSelectedActor(DeltaSeconds);
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

void ATYPlayerController::UpdateSelectedActor(float DeltaSeconds)
{
	//Tick选中Actor的位置
	if (ControllerState==EControllerState::EBuild&&SelectedActor&&GetWorld())
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForOneFrame,HitResult,true))
		{
			if (HitResult.GetActor())
			{
				SelectedActor->SetActorLocation(HitResult.GetActor()->GetActorLocation());
			}
		}
	}
	else if (ControllerState==EControllerState::EBuild&&!SelectedActor&&GetWorld())
	{
		
	}
}

void ATYPlayerController::OnClick(AActor* TargetActor)
{
	switch (ControllerState)
	{
	case EControllerState::EBuild:
		SetSelectedActor(TargetActor);
		break;
	case EControllerState::ECombat:
		if (Cast<ABaseAttacker>(TargetActor))
		{
			Cast<ABaseAttacker>(TargetActor)->Attack();
		}
		break;
	default: ;
	}
}

void ATYPlayerController::SetSelectedActor(AActor* TargetActor)
{
	check(TargetActor)
	if (!SelectedActor)
	{
		SelectedActor = TargetActor;
		UToolLibrary::DebugLog("SetNewSelectedActor");
	}
	else
	{
		SelectedActor = nullptr;
		UToolLibrary::DebugLog("SetNullSelectedActor");
	}
}
