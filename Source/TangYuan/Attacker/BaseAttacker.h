// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Pawn.h"
#include "BaseAttacker.generated.h"

UCLASS()
class TANGYUAN_API ABaseAttacker : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseAttacker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
protected:
	virtual void NotifyActorOnClicked(FKey ButtonPressed = EKeys::LeftMouseButton) override;
	
	virtual void NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex);
	
	virtual void NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex);
public:
	//是否可以攻击
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BasicAttributes")
	bool bCanAttack = false;
	//攻击之后的冷却
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BasicAttributes")
	float Cooling_Max = 2.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BasicAttributes")
	float Cooling_Current = 0.0f;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BasicAttributes")
	float Cooling_InRate = 0.01f;
	//攻击函数
	UFUNCTION(BlueprintCallable)
	void Attack();
	//重置攻击
	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	void AttackUpdate();
	//冷却定时器
	FTimerHandle TimerHandle_AttackCooling;

	//component
public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UStaticMeshComponent* ST_MeshComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	USkeletalMeshComponent* SK_MeshComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UBoxComponent* CollsionBoxComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component")
	UWidgetComponent* WidgetComp_Cooling;
};
