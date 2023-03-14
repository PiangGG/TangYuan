// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemDefinition.h"
#include "InventoryItemFragment.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "InventoryFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVE_API UInventoryFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, meta=(DeterminesOutputType=FragmentClass))
	static const UInventoryItemFragment* FindItemDefinitionFragment(TSubclassOf<UInventoryItemDefinition> ItemDef, TSubclassOf<UInventoryItemFragment> FragmentClass);
};
