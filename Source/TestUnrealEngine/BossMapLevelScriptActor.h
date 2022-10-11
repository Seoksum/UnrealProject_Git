// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "BossMapLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API ABossMapLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	ABossMapLevelScriptActor();

protected:
	virtual void BeginPlay() override;

	
private:
	UPROPERTY(VisibleAnywhere)
	class ULevelSequence* BossMapSequence;

	UPROPERTY(VisibleAnywhere)
	class ULevelSequencePlayer* SequencePlayer;

};
