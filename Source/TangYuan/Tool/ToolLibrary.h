// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TANGYUAN_API UToolLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void DebugLog(FString Message);

private:
	bool bPrintDebugLog = true;
};
