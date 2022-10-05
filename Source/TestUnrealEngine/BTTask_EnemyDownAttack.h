// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_EnemyDownAttack.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UBTTask_EnemyDownAttack : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_EnemyDownAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool bIsAttacking = false;

};
