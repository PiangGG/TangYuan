// Fill out your copyright notice in the Description page of Project Settings.


#include "TYPlayerController.h"

#include "Kismet/KismetSystemLibrary.h"
#include "TangYuan/Attacker/BaseAttacker.h"
#include "TangYuan/Map/MapUnit.h"
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
			if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor()))
			{
				SelectedActor->SetActorLocation(Cast<AMapUnit>(HitResult.GetActor())->GetBuildLocation());
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
		SelectedActorAttack(TargetActor);
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
		Cast<ABaseAttacker>(TargetActor)->OnSelected();
		UToolLibrary::DebugLog("SetNewSelectedActor");
	}
	else
	{
		SelectedActor = nullptr;
		SetActorLocationToUnitMap(TargetActor);
		Cast<ABaseAttacker>(TargetActor)->UnSelected();
		UToolLibrary::DebugLog("SetNullSelectedActor");
	}
}

void ATYPlayerController::SelectedActorAttack(AActor* TargetActor)
{
	if (Cast<ABaseAttacker>(TargetActor))
	{
		Cast<ABaseAttacker>(TargetActor)->Attack();
	}
}

void ATYPlayerController::SetActorLocationToUnitMap(AActor* TargetActor)
{
	if (TargetActor&&GetWorld())
	{
		FHitResult HitResult;
		FVector StartLocation,Direction,EndLocation;;
		DeprojectMousePositionToWorld(StartLocation,Direction);
		EndLocation = StartLocation+(Direction*LineLength);

		const TArray<AActor*> ActorsToIgnore;
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),StartLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery3,true,ActorsToIgnore,EDrawDebugTrace::ForOneFrame,HitResult,true))
		{
			if (HitResult.GetActor()&&Cast<AMapUnit>(HitResult.GetActor()))
			{
				if (Cast<ABaseAttacker>(TargetActor)->CanSetActorLocation())
				{
					TargetActor->SetActorLocation(Cast<AMapUnit>(HitResult.GetActor())->GetBuildLocation());
					Cast<ABaseAttacker>(TargetActor)->AtMapUnit = Cast<AMapUnit>(HitResult.GetActor());
				}
				else
				{
					if (Cast<ABaseAttacker>(TargetActor)->AtMapUnit)
					{
						TargetActor->SetActorLocation(Cast<ABaseAttacker>(TargetActor)->AtMapUnit->GetBuildLocation());
					}
					else
					{
						TargetActor->SetActorLocation(FVector::ZeroVector);
						UToolLibrary::DebugLog("TargetActor no AtMapUnit last");
					}
				}
			}
		}
	}
}
