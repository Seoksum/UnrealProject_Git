// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSelector.generated.h"

UCLASS()
class TESTUNREALENGINE_API ACharacterSelector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACharacterSelector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SelectCharacter(int32 Index1);

	void GreystoneNext();
	void GreystoneBefore();

	void CountessNext();
	void CountessBefore();

	void SparrowNext();
	void SparrowBefore();

	void HideGreystone();
	void HideCountess();
	void HideSparrow();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Greystones;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Countesses;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Sparrows;


	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone1;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone2;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone3;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone4;


	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess1;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess2;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess3;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess4;


	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Sparrow1;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Sparrow2;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Sparrow3;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Sparrow4;


	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> GreyStoneArray;

	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> CountessArray;

	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> SparrowArray;


	UPROPERTY(VisibleAnywhere)
	int32 NowGreystone = 0;

	UPROPERTY(VisibleAnywhere)
	int32 NowCountess = 0;

	UPROPERTY(VisibleAnywhere)
	int32 NowSparrow = 0;

	UPROPERTY(VisibleAnywhere)
	int32 CharacterMeshCount = 4;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;
};
