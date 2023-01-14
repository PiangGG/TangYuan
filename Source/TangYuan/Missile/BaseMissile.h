// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMissile.generated.h"

UCLASS()
class TANGYUAN_API ABaseMissile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseMissile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Component
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Component|移动组件")
	class UProjectileMovementComponent* MovementComponent;
	
};
