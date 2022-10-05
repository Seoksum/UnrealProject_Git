// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_EnemyWhite.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AAIController_EnemyWhite : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_EnemyWhite();

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;


private:
	FTimerHandle TimerHandle;

	UPROPERTY()
	class UBehaviorTree* BehaviorTree;

	UPROPERTY()
	class UBlackboardData* BlackboardData;

};
