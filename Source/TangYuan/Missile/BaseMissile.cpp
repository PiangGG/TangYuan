// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMissile.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABaseMissile::ABaseMissile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootCollsionComp = CreateDefaultSubobject<USphereComponent>(TEXT("RootCollsionComp"));
	RootComponent = RootCollsionComp;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(RootCollsionComp);

	ST_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_MeshComp"));
	ST_MeshComp->SetupAttachment(SceneRoot);

	SK_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MeshComp"));
	SK_MeshComp->SetupAttachment(SceneRoot);
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 512.0f;
	MovementComponent->MaxSpeed = 512.0f;
}

// Called when the game starts or when spawned
void ABaseMissile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

