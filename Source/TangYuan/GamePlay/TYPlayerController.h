// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularPlayerController.h"
#include "GameFramework/Actor.h"
#include "TangYuan/Tool/EnumLib.h"

#include "TYPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANGYUAN_API ATYPlayerController : public AModularPlayerController
{
	GENERATED_BODY()
	
	ATYPlayerController();
public:
	virtual void Tick(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Attr|射线基础长度")
	float LineLength = 10000.0f;
private:
	AActor* SelectedActor;
	
	AActor* HoverActor;
public:
	//控制器状态
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="")
	EControllerState ControllerState = EControllerState::ECombat;
	
	UFUNCTION(BlueprintCallable)
	void SetControllerState(EControllerState State);

	UFUNCTION(BlueprintCallable)
	EControllerState GetControllerState();

	void UpdateSelectedActor(float DeltaSeconds);

	/*处理鼠标点击Actor*/
	virtual void OnClick(AActor* TargetActor);

	/*设置选中Actor*/
	virtual void SetSelectedActor(AActor* TargetActor);
};
