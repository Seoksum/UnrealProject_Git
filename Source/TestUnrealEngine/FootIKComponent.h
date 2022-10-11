// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FootIKComponent.generated.h"


typedef struct ST_IK_TraceInfo
{
	float fOffset;
	FVector pImpactLocation;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTUNREALENGINE_API UFootIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFootIKComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Transient)
	class ACharacter* Character;

	UPROPERTY(Transient)
	class UMeshComponent* Mesh;

	// The name of left foot bone/socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Bone Name", meta = (AllowPrivateAccess = "true"))
	FName LeftFootSocketName;

	// The name of right foot bone/socket
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Bone Name", meta = (AllowPrivateAccess = "true"))
	FName RightFootSocketName;

	// The name of left virtual bone (add by yourself)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Bone Name", meta = (AllowPrivateAccess = "true"))
	FName LeftVirtualBoneName;

	// The name of right virtual bone (add by yourself)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Bone Name", meta = (AllowPrivateAccess = "true"))
	FName RightVirtualBoneName;

	// The name of Hip/Pelvis bone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Bone Name", meta = (AllowPrivateAccess = "true"))
	FName HipBone;

	// Z축은 발뼈와 실제 발 사이의 차이입니다. 양수 값을 입력하십시오.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Foot Height", meta = (AllowPrivateAccess = "true"))
	float FootHeight;

	FVector LeftFootLocation;
	FVector RightFootLocation;
	float HipZOffset;
	float LeftAlpha;
	float RightAlpha;

	bool bLeftHit;
	bool bRightHit;

	FHitResult LeftHit;
	FHitResult RightHit;

	// Actor that the sphere trace will ignore
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | Actors To Ignore", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> ActorsToIgnore;

	// Idle 상태의 HipOffset의 최대값. 최소값은 0.f여야 합니다. 양수 값을 입력하십시오.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | HipOffset", meta = (AllowPrivateAccess = "true"))
	float IdleHipOffsetMaxValue;

	// 이동 상태의 HipOffset의 최대값, 최소값은 0.f여야 합니다. 양수 값을 입력하십시오.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK | HipOffset", meta = (AllowPrivateAccess = "true"))
	float MovingHipOffsetMaxValue;

	// Sphere Trace for legs
	UFUNCTION()
	bool FootSphereTrace(FName FootSocket, FHitResult& Hit, float StartOffset, float EndOffset);

	// Do the calculation for legs
	UFUNCTION()
	void UpdateFootPosition(FName VirtualBone, FName FootSocket, FHitResult Hit, FVector& FootLocation, float& Alpha, float DeltaTime);

	// Do the calculation for body position
	UFUNCTION()
	void UpdateHipPosition(FHitResult LeftHitResult, FHitResult RightHitResult, float& HipOffset, float DeltaTime);

	// Clamping the Hip Offset
	UFUNCTION()
	float ClampHipOffset(float X, float Min, float Max);

	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetLeftFootEffector() { return LeftFootLocation; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetRightFootEffector() { return RightFootLocation; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetLeftFootAlpha() { return LeftAlpha; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRightFootAlpha() { return RightAlpha; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetHipOffset() { return HipZOffset; }

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};
