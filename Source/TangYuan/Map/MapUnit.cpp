// Fill out your copyright notice in the Description page of Project Settings.


#include "MapUnit.h"

#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMapUnit::AMapUnit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = RootComp;

	CollsionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionComp"));
	CollsionComp->SetupAttachment(RootComp);
	CollsionComp->SetBoxExtent(BoxExtent);
	CollsionComp->SetRelativeLocation(FVector(0.0f,0.0f,BoxExtent.Z));
	CollsionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollsionComp->SetCollisionProfileName(FName("MapUnitBox"));
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComp);
	MeshComponent->SetRelativeLocation(FVector(0.0f,0.0f,1.0f));
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->SetCollisionProfileName(FName("MapUnitMesh"));
	MeshComponent->SetStaticMesh(LoadObject<UStaticMesh>(nullptr,TEXT("/Script/Engine.StaticMesh'/Game/Mesh_ST/Mesh/Plane.Plane'")));
	
	Material_G  = LoadObject<UMaterialInterface>(nullptr,TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Map/M_MapUnitColor_G.M_MapUnitColor_G'"));
	Material_R  = LoadObject<UMaterialInterface>(nullptr,TEXT("/Script/Engine.MaterialInstanceConstant'/Game/Materials/Map/M_MapUnitColor_R.M_MapUnitColor_R'"));
	MeshComponent->SetMaterial(0,Material_G);
}

FVector AMapUnit::GetBuildLocation()
{
	return  CollsionComp->GetComponentLocation();
}

// Called when the game starts or when spawned
void AMapUnit::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateUnitMateria(DeltaTime);
}

void AMapUnit::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MeshComponent)
	{
		MeshComponent->SetVisibility(false);
	}
}

void AMapUnit::SetUnitVisble(bool bShow)
{
	bShowStaticMesh = bShow;
	if (bShowStaticMesh)
	{
		//MeshComponent->SetMaterial(0,);
		TArray<AActor*>Actors;
		CollsionComp->GetOverlappingActors(Actors);
		if (Actors.Num()>0)
		{
			MeshComponent->SetMaterial(0,Material_R);
		}
		else
		{
			MeshComponent->SetMaterial(0,Material_G);
		}
	}
	MeshComponent->SetVisibility(bShowStaticMesh);
}

bool AMapUnit::GetUnitVisble()
{
	return bShowStaticMesh;
}

void AMapUnit::UpdateUnitMateria(float DeltaTime)
{
	if (bShowStaticMesh)
	{
		TArray<AActor*>Actors;
		CollsionComp->GetOverlappingActors(Actors);
		if (Actors.Num()>0)
		{
			MeshComponent->SetMaterial(0,Material_R);
		}
		else
		{
			MeshComponent->SetMaterial(0,Material_G);
		}
	}
}

bool AMapUnit::GetbOverlapActor(AActor* ignoreActor)
{
	TArray<AActor*> Actors;
	Actors.Add(ignoreActor);
	TArray<FHitResult> HitResults;
	UKismetSystemLibrary::BoxTraceMulti(GetWorld(),GetBuildLocation(),GetBuildLocation(),BoxExtent,FRotator(0),ETraceTypeQuery::TraceTypeQuery4,false,Actors,EDrawDebugTrace::ForDuration,HitResults,true);

	return !(HitResults.IsEmpty());
}

