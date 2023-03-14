// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InventoryItemFragment.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemDefinition.generated.h"

/**
 * 
 */
UCLASS()
class INTERACTIVE_API UInventoryItemDefinition : public UObject
{
	GENERATED_BODY()

public:
	UInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display, Instanced)
	TArray<TObjectPtr<UInventoryItemFragment>> Fragments;

public:
	const UInventoryItemFragment* FindFragmentByClass(TSubclassOf<UInventoryItemFragment> FragmentClass) const;
};
