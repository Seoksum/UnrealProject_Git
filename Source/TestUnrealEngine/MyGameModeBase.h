// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMyGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

protected:
	virtual void PostInitializeComponents() override;
	
	
public:
	void SetNowIndex(int32 Index);

	


public:
	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	UPROPERTY()
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(VisibleAnywhere)
	int32 NowIndex;

	UPROPERTY(VisibleAnywhere)
	int32 CharMeshIndex;

	UPROPERTY(VisibleAnywhere)
	int32 MaterialIndex=0;
	

};
