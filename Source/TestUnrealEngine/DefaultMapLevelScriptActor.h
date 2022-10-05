// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "DefaultMapLevelScriptActor.generated.h"


DECLARE_MULTICAST_DELEGATE(FOpenDoor);
DECLARE_MULTICAST_DELEGATE(FOnKillDusk);
DECLARE_MULTICAST_DELEGATE(FOnKillDown);
DECLARE_MULTICAST_DELEGATE(FOnKillBuff);

/**
 *
 */
UCLASS()
class TESTUNREALENGINE_API ADefaultMapLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ADefaultMapLevelScriptActor();


protected:
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	int32 CatchDuskCount = 0;

	UPROPERTY(EditAnywhere)
	int32 CatchDownCount = 0;

	UPROPERTY(EditAnywhere)
	int32 CatchWhiteCount = 0;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy_Dusk> FindEnemyDuskType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy_Down> FindEnemyDownType;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AEnemy_Buff_White> FindEnemyWhiteType;

	void CatchDusk();
	void CatchDown();
	void CatchWhite();

	FOnKillDusk OnKillDusk;
	FOnKillDown OnKillDown;
	FOnKillBuff OnKillBuff;



};