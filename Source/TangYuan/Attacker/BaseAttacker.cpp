// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttacker.h"

// Sets default values
ABaseAttacker::ABaseAttacker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollsionBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBoxComp"));
	RootComponent = CollsionBoxComp;

	ST_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_MeshComp"));
	ST_MeshComp->SetupAttachment(CollsionBoxComp);

	SK_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MeshComp"));
	SK_MeshComp->SetupAttachment(CollsionBoxComp);

	WidgetComp_Cooling = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp_Cooling"));
	WidgetComp_Cooling->SetupAttachment(CollsionBoxComp);
}

// Called when the game starts or when spawned
void ABaseAttacker::BeginPlay()
{
	Super::BeginPlay();
	
	ResetAttack();
}

// Called every frame
void ABaseAttacker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseAttacker::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseAttacker::NotifyActorOnClicked(FKey ButtonPressed)
{
	Super::NotifyActorOnClicked(ButtonPressed);
	UE_LOG(LogTemp, Warning, TEXT("Clicked"));
	Attack();
}

void ABaseAttacker::NotifyActorOnInputTouchBegin(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchBegin(FingerIndex);
	UE_LOG(LogTemp, Warning, TEXT("TouchBegin"));
}

void ABaseAttacker::NotifyActorOnInputTouchEnd(const ETouchIndex::Type FingerIndex)
{
	Super::NotifyActorOnInputTouchEnd(FingerIndex);
	UE_LOG(LogTemp, Warning, TEXT("TouchEnd"));
}

void ABaseAttacker::Attack()
{
	if (bCanAttack)
	{
		ResetAttack();
	}
}

void ABaseAttacker::ResetAttack()
{
	bCanAttack = false;
	Cooling_Current = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackCooling,this,&ABaseAttacker::AttackUpdate,0.001,true);
}

void ABaseAttacker::AttackUpdate()
{
	//Cooling_Current
	Cooling_Current = FMath::FInterpTo(Cooling_Current,Cooling_Max,GetWorld()->GetTimeSeconds(),Cooling_InRate);
	if (FMath::IsNearlyEqual(Cooling_Current,Cooling_Max,Cooling_InRate))
	{
		bCanAttack = true;
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle_AttackCooling);
	}
}

