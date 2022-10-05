// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS()
class TESTUNREALENGINE_API AInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInteractable();

	void AA();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintNativeEvent)
		void Interact();
	virtual void Interact_Implementation();

	// 모든 상호작용가능한 것들은 static mesh를 가진다고 가정
	/*UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		class UStaticMeshComponent* InteractableMesh;*/

	// 도움 메시지, e를 눌러서 주워라 이런 메시지
	UPROPERTY(EditAnywhere, Category = "Interactable Properties")
		FString InteractableHelpText;

};
