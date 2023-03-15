// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayAbilityTargetData_SingleTargetHit.h"
struct FGameplayEffectContextHandle;

//////////////////////////////////////////////////////////////////////

void FBaseGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(FGameplayEffectContextHandle& Context, bool bIncludeActorArray) const
{
	FGameplayAbilityTargetData_SingleTargetHit::AddTargetDataToContext(Context, bIncludeActorArray);

	// Add game-specific data
	// if (FBaseGameplayEffectContext* TypedContext = FGameplayEffectContext::ExtractEffectContext(Context))
	// {
	// 	TypedContext->CartridgeID = CartridgeID;
	// }
}

bool FBaseGameplayAbilityTargetData_SingleTargetHit::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayAbilityTargetData_SingleTargetHit::NetSerialize(Ar, Map, bOutSuccess);

	Ar << CartridgeID;

	return true;
}