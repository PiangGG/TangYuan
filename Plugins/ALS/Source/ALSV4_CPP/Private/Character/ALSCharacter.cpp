// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSCharacter.h"

#include "Engine/StaticMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"

AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	AIControllerClass = AALSAIController::StaticClass();

	PhysicsControlComponent = CreateDefaultSubobject<UPhysicsControlComponent>(TEXT("PhysicsControlComponent"));
}

void AALSCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass,
                                 bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	if (IsValid(NewStaticMesh))
	{
		StaticMesh->SetStaticMesh(NewStaticMesh);
	}
	else if (IsValid(NewSkeletalMesh))
	{
		SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
		if (IsValid(NewAnimClass))
		{
			SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
		}
	}

	FName AttachBone;
	if (bLeftHand)
	{
		AttachBone = TEXT("VB LHS_ik_hand_gun");
	}
	else
	{
		AttachBone = TEXT("VB RHS_ik_hand_gun");
	}

	HeldObjectRoot->AttachToComponent(GetMesh(),
	                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	HeldObjectRoot->SetRelativeLocation(Offset);
}

void AALSCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void AALSCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
	EnsureControls();
}

void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHeldObject();
}

void AALSCharacter::MakeControls()
{
	NeedToMakeControls = false;

	GetMesh()->SetConstraintProfileForAll(FName("HingesOnly"),false);

	FPhysicsControlData PhysicsControlData;
	PhysicsControlData.LinearStrength = SKLinearStrength;
	PhysicsControlData.AngularStrength = SKAngularStrength;
	SKLimbControls = PhysicsControlComponent->MakeControlsFromLimbBones(SKAllControls,PhysicsControlComponent->GetLimbBonesFromSkeletalMesh(GetMesh(),LimbSetupData),EPhysicsControlType::WorldSpace,PhysicsControlData,FPhysicsControlSettings(),false);

	FPhysicsControlData PhysicsControlData2;
	PhysicsControlData2.LinearStrength = 0.0f;
	PhysicsControlData2.AngularStrength = AAStrength;
	AALimbControls = PhysicsControlComponent->MakeControlsFromLimbBones(AAAllControls,PhysicsControlComponent->GetLimbBonesFromSkeletalMesh(GetMesh(),LimbSetupData),EPhysicsControlType::ParentSpace,PhysicsControlData2,FPhysicsControlSettings(),false);

	LimbBodyModifiers = PhysicsControlComponent->MakeBodyModifiersFromLimbBones(AllBodyModifiers,PhysicsControlComponent->GetLimbBonesFromSkeletalMesh(GetMesh(),LimbSetupData),EPhysicsMovementType::Static,ECollisionEnabled::QueryAndPhysics,0.0f,true);
}

void AALSCharacter::GetNoiseVector(float Amplitude, float Frequency, FVector& ReturnValue, float& Magnitude)
{
	float CurrentTime;
	CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	ReturnValue = FVector(FMath::Sin(CurrentTime*(Frequency*0.7)),FMath::Sin(CurrentTime*(Frequency*1.0)),FMath::Sin(CurrentTime*(Frequency*1.3)));
	Magnitude = ReturnValue.Size();
}

void AALSCharacter::GetPerlinNoiseVector(float Amplitude, float Frequency, float TimeOffset, FVector& ReturnValue)
{
	float CurrentTime;
	CurrentTime = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	FVector TempVector(
	FMath::PerlinNoise1D((CurrentTime+TimeOffset)*Frequency),
	FMath::PerlinNoise1D((CurrentTime+TimeOffset)*Frequency+0.3333),
	FMath::PerlinNoise1D((CurrentTime+TimeOffset)*Frequency+0.6666));
	ReturnValue = TempVector*Amplitude;
}

void AALSCharacter::EnsureControls()
{
	if (NeedToMakeControls)
	{
		MakeControls();
		
		PhysicsControlComponent ->SetAllBodyModifierMovementType(AllBodyModifiers.Names,EPhysicsMovementType::Simulated);

		FollowAnimation();
	}
}

void AALSCharacter::FollowAnimation()
{
	PhysicsControlComponent->SetAllControlsEnabled(SKAllControls.Names,true);
	GetMesh()->SetConstraintProfileForAll(FName("HingesOnly"),false);
	PhysicsControlComponent->SetAllBodyModifierGravityMultipliers(AllBodyModifiers.Names,0.0f);
	PhysicsControlComponent->SetBodyModifierMovementType(FName("foot_l_0"),EPhysicsMovementType::Kinematic);
	PhysicsControlComponent->SetBodyModifierMovementType(FName("foot_r_0"),EPhysicsMovementType::Kinematic);
	GetMesh()->SetPlayRate(1.0f);
}

void AALSCharacter::Ragdoll()
{
	PhysicsControlComponent->SetAllControlsEnabled(SKAllControls.Names,false);
	GetMesh()->SetConstraintProfileForAll(FName(""),true);
	PhysicsControlComponent->SetAllBodyModifierGravityMultipliers(AllBodyModifiers.Names,1.0f);
	PhysicsControlComponent->SetBodyModifierMovementType(FName("foot_l_0"),EPhysicsMovementType::Simulated);
	PhysicsControlComponent->SetBodyModifierMovementType(FName("foot_r_0"),EPhysicsMovementType::Simulated);
	GetMesh()->SetPlayRate(0.0f);
}
