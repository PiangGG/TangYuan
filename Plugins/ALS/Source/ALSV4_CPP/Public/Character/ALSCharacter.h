// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"
#include "PhysicsControl/Public/PhysicsControlComponent.h"
#include "ALSCharacter.generated.h"

/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter
{
	GENERATED_BODY()

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh,
	                  class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

private:
	bool bNeedsColorReset = false;
public:
	//PhysicsControlComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|Component")
	UPhysicsControlComponent* PhysicsControlComponent;
	//Var
	//SK Controls
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|SK Controls")
	float SKLinearStrength = 3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|SK Controls")
	float SKAngularStrength = 3.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|SK Controls")
	TMap<FName,FPhysicsControlNameArray>SKLimbControls;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|SK Controls")
	FPhysicsControlNameArray SKAllControls;
	//AA Controls
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|AA Controls")
	TMap<FName,FPhysicsControlNameArray>AALimbControls;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|AA Controls")
	float AAStrength = 10.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|AA Controls")
	FPhysicsControlNameArray AAAllControls;
	//Body Modifiers
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|Body Modifiers")
	FPhysicsControlNameArray AllBodyModifiers;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie|Body Modifiers")
	TMap<FName,FPhysicsControlNameArray> LimbBodyModifiers;
	//配置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category="Zombie|配置")
	TArray<FPhysicsControlLimbSetupData>LimbSetupData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category="Zombie")
	bool NeedToMakeControls = true;
	//void InitPhysicsControlComponent();
	UFUNCTION(BlueprintCallable)
	void MakeControls();
	UFUNCTION(BlueprintCallable)
	void GetNoiseVector(float Amplitude,float Frequency,FVector &ReturnValue,float &Magnitude);
	UFUNCTION(BlueprintCallable)
	void GetPerlinNoiseVector(float Amplitude,float Frequency,float TimeOffset,FVector &ReturnValue);
	UFUNCTION(BlueprintCallable)
	void EnsureControls();
	
	UFUNCTION(BlueprintCallable)
	void FollowAnimation();
	UFUNCTION(BlueprintCallable)
	void Ragdoll();
};
