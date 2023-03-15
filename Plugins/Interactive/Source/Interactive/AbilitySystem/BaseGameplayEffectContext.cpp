// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGameplayEffectContext.h"

FBaseGameplayEffectContext* FBaseGameplayEffectContext::ExtractEffectContext(FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FBaseGameplayEffectContext::StaticStruct()))
	{
		return (FBaseGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FBaseGameplayEffectContext::SetAbilitySource(const IAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IAbilitySourceInterface* FBaseGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IAbilitySourceInterface>(AbilitySourceObject.Get());
}

bool FBaseGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	FGameplayEffectContext::NetSerialize(Ar, Map, bOutSuccess);

	// Not serialized for post-activation use:
	// CartridgeID

	return true;
}

const UPhysicalMaterial* FBaseGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

#if UE_WITH_IRIS
namespace UE::Net
{
	// Forward to FGameplayEffectContextNetSerializer
	// Note: If FLyraGameplayEffectContext::NetSerialize() is modified, a custom NetSerializesr must be implemented as the current fallback will no longer be sufficient.
	UE_NET_IMPLEMENT_FORWARDING_NETSERIALIZER_AND_REGISTRY_DELEGATES(LyraGameplayEffectContext, FGameplayEffectContextNetSerializer);
}
#endif