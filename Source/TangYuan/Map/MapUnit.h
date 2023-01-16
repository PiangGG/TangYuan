// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MapUnit.generated.h"

UCLASS()
class TANGYUAN_API AMapUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapUnit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;
	//Component
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|SceneComponent")
	USceneComponent* RootComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|CollsionComponent")
	UBoxComponent* CollsionComp;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Comonent|MeshComponent")
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|ShowMesh")
	bool bShowStaticMesh = false;
	UFUNCTION(BlueprintCallable)
	void SetUnitVisble(bool bShow);
	UFUNCTION(BlueprintCallable)
	bool GetUnitVisble();

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Base|ShowMesh")
	FVector BoxExtent = FVector(50.0);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_G;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Base|Material")
	UMaterialInterface *Material_R;
};
