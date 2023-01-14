// Fill out your copyright notice in the Description page of Project Settings.


#include "TYMapGameInstanceSubsystem.h"
#include "MapUnit.h"
#include "Kismet/GameplayStatics.h"

UTYMapGameInstanceSubsystem::UTYMapGameInstanceSubsystem()
{
	
}

void UTYMapGameInstanceSubsystem::SetUnitVisble(bool bVisble)
{
	/*if (bVisble)
	{
		
	}
	else
	{
		
	}*/
	TArray<AActor*>Units;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AMapUnit::StaticClass(),Units);
	for (auto& Unit : Units)
	{
		Cast<AMapUnit>(Unit)->SetUnitVisble(bVisble);
	}
}
