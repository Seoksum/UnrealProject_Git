// Fill out your copyright notice in the Description page of Project Settings.


#include "FootIKComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Math/UnrealMathUtility.h" 
#include "MyCharacter.h" 
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UFootIKComponent::UFootIKComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	LeftFootSocketName = "foot_l";
	RightFootSocketName = "foot_r";
	LeftVirtualBoneName = "VB pelvis_foot_l";
	RightVirtualBoneName = "VB pelvis_foot_r";
	HipBone = "pelvis";

	LeftFootLocation = FVector(0.f);
	RightFootLocation = FVector(0.f);
	HipZOffset = 0.f;
	LeftAlpha = 0.f;
	RightAlpha = 0.f;

	bLeftHit = false;
	bRightHit = false;

	IdleHipOffsetMaxValue = 40.f;
	MovingHipOffsetMaxValue = 30.f;

	FootHeight = 13.f;
}


// Called when the game starts
void UFootIKComponent::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AMyCharacter>(GetOwner());

	if (Character)
	{
		Mesh = Character->GetMesh();
	}
	
}

// Called every frame
void UFootIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bLeftHit = FootSphereTrace(LeftFootSocketName, LeftHit, 0.f, -200.f);
	bRightHit = FootSphereTrace(RightFootSocketName, RightHit, 0.f, -200.f);

	// Adjust the position of the hip
	UpdateHipPosition(LeftHit, RightHit, HipZOffset, DeltaTime);

	// Change the legs' placement
	UpdateFootPosition(LeftVirtualBoneName, LeftFootSocketName, LeftHit, LeftFootLocation, LeftAlpha, DeltaTime);
	UpdateFootPosition(RightVirtualBoneName, RightFootSocketName, RightHit, RightFootLocation, RightAlpha, DeltaTime);
}

bool UFootIKComponent::FootSphereTrace(FName FootSocket, FHitResult& Hit, float StartOffset, float EndOffset)
{
	FVector SocketLocation = Mesh->GetSocketLocation(FootSocket);
	return UKismetSystemLibrary::SphereTraceSingle
	(
		this,
		FVector(SocketLocation.X, SocketLocation.Y, Character->GetActorLocation().Z + StartOffset),
		FVector(SocketLocation.X, SocketLocation.Y, Character->GetActorLocation().Z + EndOffset),
		5.f,
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		Hit,
		true
	);
}


void UFootIKComponent::UpdateFootPosition(FName VirtualBone, FName FootSocket, FHitResult Hit, FVector& FootLocation, float& Alpha, float DeltaTime)
{
	//UpdateFootPosition(LeftVirtualBoneName, LeftFootSocketName, LeftHit, LeftFootLocation, LeftAlpha, DeltaTime);
	//UpdateFootPosition(RightVirtualBoneName, RightFootSocketName, RightHit, RightFootLocation, RightAlpha, DeltaTime);
	const FVector VirtualBoneLocation = Mesh->GetSocketLocation(VirtualBone);
	const FVector FootPosition = Mesh->GetSocketLocation(FootSocket);

	float FootZPosition = 0.f;

	// check if the impact point is higher than the foot
	if (Hit.ImpactPoint.Z > FootPosition.Z - FootHeight)
	{
		// Set the the foot height 1.f less than the ImpactPoint checking, to prevent Alpha looping at 0 and 1 
		FootLocation = FVector(VirtualBoneLocation.X, VirtualBoneLocation.Y, Hit.ImpactPoint.Z + (FootHeight - 1.f));

		Alpha = FMath::FInterpTo(Alpha, 1.f, DeltaTime, 6.f);
	}
	else
	{
		Alpha = FMath::FInterpTo(Alpha, 0.f, DeltaTime, 6.f);
	}
}

void UFootIKComponent::UpdateHipPosition(FHitResult LeftHitResult, FHitResult RightHitResult, float& HipOffset, float DeltaTime)
{

	// UpdateHipPosition(LeftHit, RightHit, HipZOffset, DeltaTime);
	if (bLeftHit && bRightHit)
	{
		if (Character->GetVelocity().Size() > 0)
		{
			if (LeftHit.ImpactPoint.Z != RightHit.ImpactPoint.Z)
			{
				float ClampedOffset = ClampHipOffset(FMath::Abs(LeftHit.ImpactPoint.Z - RightHit.ImpactPoint.Z), 0.f, MovingHipOffsetMaxValue);
				HipOffset = FMath::FInterpTo(HipOffset, -ClampedOffset, DeltaTime, 20.f);
			}
			else
			{
				HipOffset = FMath::FInterpTo(HipOffset, 0.f, DeltaTime, 20.f);

			}
		}
		else
		{
			if (LeftHit.ImpactPoint.Z != RightHit.ImpactPoint.Z)
			{
				float ClampedOffset = ClampHipOffset(FMath::Abs(LeftHit.ImpactPoint.Z - RightHit.ImpactPoint.Z), 0.f, IdleHipOffsetMaxValue);
				HipOffset = FMath::FInterpTo(HipOffset, -ClampedOffset, DeltaTime, 20.f);
			}
			else
			{
				HipOffset = FMath::FInterpTo(HipOffset, 0.f, DeltaTime, 20.f);
			}
		}
	}
	return;
}

float UFootIKComponent::ClampHipOffset(float X, float Min, float Max)
{
	return FMath::Clamp(X, Min, Max);
}




