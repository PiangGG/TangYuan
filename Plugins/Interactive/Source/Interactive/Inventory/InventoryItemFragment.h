// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemInstance.h"
#include "InventoryItemFragment.generated.h"

/**
 * 
 */
// Represents a fragment of an item definition
UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class INTERACTIVE_API UInventoryItemFragment : public UObject
{
	GENERATED_BODY()

public:
	virtual void OnInstanceCreated(UInventoryItemInstance* Instance) const {}
};
