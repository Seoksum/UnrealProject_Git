// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Title_PC.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API ATitle_PC : public APlayerController
{
	GENERATED_BODY()


public:
	ATitle_PC();

	virtual void BeginPlay() override;

	void CreateServer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UStartMenuWidget> TitleUIClass;

private:
	UPROPERTY()
	class UStartMenuWidget* TitleWidget;

};
