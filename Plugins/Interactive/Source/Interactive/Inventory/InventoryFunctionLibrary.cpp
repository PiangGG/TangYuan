// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryFunctionLibrary.h"

const UInventoryItemFragment* UInventoryFunctionLibrary::FindItemDefinitionFragment(
	TSubclassOf<UInventoryItemDefinition> ItemDef, TSubclassOf<UInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}
