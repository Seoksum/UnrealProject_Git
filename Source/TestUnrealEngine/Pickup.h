// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API APickup : public AInteractable
{
	GENERATED_BODY()
	
public:
	APickup();

	virtual void BeginPlay() override;

	virtual void Interact_Implementation() override;

	UFUNCTION(BlueprintNativeEvent)
	void Use(); // 인벤토리 아이템 사용
	virtual void Use_Implementation();

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	UTexture2D* PickupThumbnail;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	FString ItemName;

	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	int32 Value; // 아이템 가치

	// 아이템이 픽업될 때 호출되는 함수
	void OnPickedUp();


	UPROPERTY(EditAnywhere, Category = "Pickup Properties")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(VisibleDefaultsOnly)
	class USphereComponent* CollisionComp;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
