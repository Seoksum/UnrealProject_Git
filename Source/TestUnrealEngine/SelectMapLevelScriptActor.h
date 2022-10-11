// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "SelectMapLevelScriptActor.generated.h"

/**
 * 
 */


DECLARE_MULTICAST_DELEGATE(FOpenDoor);
UCLASS()
class TESTUNREALENGINE_API ASelectMapLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ASelectMapLevelScriptActor();


protected:
	virtual void BeginPlay() override;


public:

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	FTimerHandle DoorTimerHandle;

	FOpenDoor OpenDoor;

	UFUNCTION()
	void DestroyDoor();

private:
	UPROPERTY(VisibleAnywhere)
	class ULevelSequence* FirstMapSequence;

	UPROPERTY(VisibleAnywhere)
	class ULevelSequencePlayer* SequencePlayer;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AFountain> FindDoorType;

	
	
};
