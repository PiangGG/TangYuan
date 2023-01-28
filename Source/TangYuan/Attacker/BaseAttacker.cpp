// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttacker.h"

#include "TangYuan/Missile/BaseMissile.h"

// Sets default values
ABaseAttacker::ABaseAttacker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollsionBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBoxComp"));
	CollsionBoxComp->SetBoxExtent(FVector(45.0f));
	RootComponent = CollsionBoxComp;

	MeshRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRootComp"));
	MeshRootComponent->SetupAttachment(CollsionBoxComp);
	
	ST_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_MeshComp"));
	ST_MeshComp->SetupAttachment(MeshRootComponent);
	ST_MeshComp->OnClicked.AddDynamic(this,&ABaseAttacker::OnClicked);

	SK_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MeshComp"));
	SK_MeshComp->SetupAttachment(MeshRootComponent);
	SK_MeshComp->OnClicked.AddDynamic(this,&ABaseAttacker::OnClicked);

	AttackLocationComp = CreateDefaultSubobject<USceneComponent>(TEXT("AttackLocationComp"));
	AttackLocationComp->SetupAttachment(MeshRootComponent);
	
	WidgetComp_Cooling = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp_Cooling"));
	WidgetComp_Cooling->SetupAttachment(CollsionBoxComp);

	ArrowComponent =  CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComp"));
	ArrowComponent->SetupAttachment(MeshRootComponent);

}

// Called when the game starts or when spawned
void ABaseAttacker::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_Change,this,&ABaseAttacker::Change,Cooling_InRate,true);
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

		GetWorld()->SpawnActor<ABaseMissile>(Missile,AttackLocationComp->GetComponentLocation(),AttackLocationComp->GetComponentRotation());
	}
}

void ABaseAttacker::ResetAttack()
{
	bCanAttack = false;
	Cooling_Current = 0.0f;
	//每帧执行攻击冷却定时器
	GetWorldTimerManager().SetTimer(TimerHandle_AttackCooling,this,&ABaseAttacker::AttackUpdate,Cooling_InRate,true);
	//冷却最大时间后执行
	GetWorldTimerManager().SetTimer(TimerHandle_ReSetAttackCool,this,&ABaseAttacker::ResetAttackTimeHandler,Cooling_Max,false);
}

void ABaseAttacker::ResetAttackTimeHandler()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_AttackCooling);
	GetWorldTimerManager().ClearTimer(TimerHandle_ReSetAttackCool);
}

void ABaseAttacker::AttackUpdate()
{
	
	Cooling_Current = FMath::Clamp(Cooling_Max-GetWorldTimerManager().GetTimerRemaining(TimerHandle_ReSetAttackCool),0.0f,Cooling_Max);
	Cooling_Alpha = Cooling_Current/Cooling_Max;
	if (FMath::IsNearlyEqual(Cooling_Alpha,1.0f,Cooling_InRate))
	{
		bCanAttack = true;
	}
}

void ABaseAttacker::Change()
{
	FRotator TagetRotater(0.0,ChangeSize,0.0);
	
	//AddActorLocalRotation(TagetRotater);
	MeshRootComponent->AddLocalRotation(TagetRotater);
}

void ABaseAttacker::OnClicked(UPrimitiveComponent* ClickedComp,FKey Key)
{
	int i = 1;
}