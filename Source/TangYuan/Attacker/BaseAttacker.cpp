// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttacker.h"

#include "TangYuan/GamePlay/TYPlayerController.h"
#include "TangYuan/Map/MapUnit.h"
#include "TangYuan/Missile/BaseMissile.h"
#include "TangYuan/Tool/ToolLibrary.h"

// Sets default values
ABaseAttacker::ABaseAttacker()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollsionBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollsionBoxComp"));
	CollsionBoxComp->SetBoxExtent(FVector(CollSionSize));
	CollsionBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollsionBoxComp->SetCollisionProfileName(FName("AttackerCollsion"));
	RootComponent = CollsionBoxComp;
	
	MeshRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("MeshRootComp"));
	MeshRootComponent->SetupAttachment(CollsionBoxComp);
	
	ST_MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ST_MeshComp"));
	ST_MeshComp->SetupAttachment(MeshRootComponent);
	ST_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ST_MeshComp->SetCollisionProfileName(FName("AttackerMesh"));
	ST_MeshComp->bCastDynamicShadow = false;
	ST_MeshComp->bCastStaticShadow = false;
	//ST_MeshComp->OnClicked.AddDynamic(this,&ABaseAttacker::OnClicked);

	SK_MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SK_MeshComp"));
	SK_MeshComp->SetupAttachment(MeshRootComponent);
	SK_MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SK_MeshComp->SetCollisionProfileName(FName("AttackerMesh"));
	SK_MeshComp->bCastDynamicShadow = false;
	SK_MeshComp->bCastStaticShadow = false;
	//SK_MeshComp->OnClicked.AddDynamic(this,&ABaseAttacker::OnClicked);

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
	
	ResetAttack();

	CollsionBoxComp->OnComponentBeginOverlap.AddDynamic(this,&ThisClass::OnCollsionBoxCompBeginOverlap);

	CollsionBoxComp->OnComponentEndOverlap.AddDynamic(this,&ThisClass::OnCollsionBoxCompEndOverlap);
	//AutoSetLocation();
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
	
	UToolLibrary::DebugLog("Clicked");
	if (GetWorld())
	{
		ATYPlayerController * PC = Cast<ATYPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->OnClick(this);
		}
	}
	//Attack();
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

		GetWorldTimerManager().ClearTimer(TimerHandle_Change);
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
	GetWorldTimerManager().SetTimer(TimerHandle_Change,this,&ABaseAttacker::Change,Cooling_InRate,true);
	
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
	
	if (bCanChange)
	{
		//AddActorLocalRotation(TagetRotater);
		MeshRootComponent->AddLocalRotation(TagetRotater);
	}
}

void ABaseAttacker::OnSelected()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Change);
	//CollsionBoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseAttacker::UnSelected()
{
	ResetAttack();
	//CollsionBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ABaseAttacker::OnClicked(UPrimitiveComponent* ClickedComp,FKey Key)
{
	int i = 1;
}

void ABaseAttacker::OnCollsionBoxCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<AMapUnit>(OtherActor))
	{
	UToolLibrary::DebugLog("OnCollsionBoxCompBeginOverlap");
		Cast<AMapUnit>(OtherActor)->SetUnitVisble(true);
	}
}

void ABaseAttacker::OnCollsionBoxCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AMapUnit>(OtherActor))
	{
		UToolLibrary::DebugLog("OnCollsionBoxCompEndOverlap");
		Cast<AMapUnit>(OtherActor)->SetUnitVisble(false);
	}
}

void ABaseAttacker::AutoSetLocation()
{
	
	CollsionBoxComp->GetOverlappingActors(OverlapMapUnit,AMapUnit::StaticClass());

	if (!OverlapMapUnit.IsEmpty())
	{
		FVector InitLocation = GetActorLocation();

		double Distence = FVector::Distance(InitLocation,Cast<AMapUnit>(OverlapMapUnit[0])->GetBuildLocation());

		AtMapUnit = Cast<AMapUnit>(OverlapMapUnit[0]);
		
		for (AActor *MapUnit : OverlapMapUnit)
		{
			if (Distence>FVector::Distance(InitLocation,Cast<AMapUnit>(MapUnit)->GetBuildLocation()))
			{
				Distence = FVector::Distance(InitLocation,Cast<AMapUnit>(MapUnit)->GetBuildLocation());

				AtMapUnit = Cast<AMapUnit>(MapUnit);
			}
			Cast<AMapUnit>(MapUnit)->SetOnActor(this);
		}
		if (AtMapUnit)
		{
			this->SetActorLocation(AtMapUnit->GetBuildLocation());

			for (auto MapUnit : OverlapMapUnit)
			{
				Cast<AMapUnit>(MapUnit)->SetUnitVisble(false);
				Cast<AMapUnit>(MapUnit)->SetOnActor(this);
			}
		}
	}
}

bool ABaseAttacker::CanSetActorLocation()
{
	TArray<AActor*> OverlapUnitMaps;
	GetOverlappingActors(OverlapUnitMaps,AMapUnit::StaticClass());

	for (auto OverlapUnitMap : OverlapUnitMaps)
	{
		if (Cast<AMapUnit>(OverlapUnitMap)->GetbOverlapActor(this))
		{
			return false;
		}
	}
	return true;
}

void ABaseAttacker::SetOverlapMapUnitShow()
{
	TArray<AActor*> OverlapUnitMaps;
	GetOverlappingActors(OverlapUnitMaps,AMapUnit::StaticClass());

	for (auto OverlapUnitMap : OverlapUnitMaps)
	{
		if (Cast<AMapUnit>(OverlapUnitMap)->GetbOverlapActor(this))
		{
			
		}
	}
}
