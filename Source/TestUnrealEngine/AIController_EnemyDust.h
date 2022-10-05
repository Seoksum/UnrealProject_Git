// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIController_EnemyDust.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AAIController_EnemyDust : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIController_EnemyDust();

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
