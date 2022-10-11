// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "MyCharacter.h"
#include "Enemy_Dusk.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyDusk = Cast<AEnemy_Dusk>(OwnerComp.GetAIOwner()->GetPawn());

	if (EnemyDusk == nullptr)
		return EBTNodeResult::Failed;
	
	if(EnemyDusk->IsDeath)
		return EBTNodeResult::Failed;
	
	EnemyDusk->Attack();

	bIsAttacking = true;

	EnemyDusk->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});


	return Result;

}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

