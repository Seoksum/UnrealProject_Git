// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSelectController.generated.h"

/**
 *
 */
UCLASS()
class TESTUNREALENGINE_API ACharacterSelectController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterSelectController();

	virtual void BeginPlay() override;


	void CreateCharacterMenu();
	void HideCharacterMenu();


public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class USelectWidget> BP_SelectWidget;



private:
	UPROPERTY()
	class USelectWidget* SelectWidget;



};
