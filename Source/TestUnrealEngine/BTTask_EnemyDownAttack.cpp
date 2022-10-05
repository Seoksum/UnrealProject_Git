// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyDownAttack.h"
#include "AIController_EnemyDust.h"
#include "Enemy_Down.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_EnemyDownAttack::UBTTask_EnemyDownAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyDownAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyDown = Cast<AEnemy_Down>(OwnerComp.GetAIOwner()->GetPawn());
	auto Greystone = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (EnemyDown == nullptr)
		return EBTNodeResult::Failed;

	if (EnemyDown->IsDeath)
		return EBTNodeResult::Failed;

	if (Greystone)
	{
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(EnemyDown->GetActorLocation(), Greystone->GetActorLocation());
		EnemyDown->SetActorRotation(newRotation);
	}

	EnemyDown->Attack();

	bIsAttacking = true;

	EnemyDown->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}


void UBTTask_EnemyDownAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}