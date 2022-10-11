// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyWhiteAttack.h"
#include "AIController_EnemyWhite.h"
#include "Enemy_Buff_White.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_EnemyWhiteAttack::UBTTask_EnemyWhiteAttack()
{
	bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_EnemyWhiteAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyWhite = Cast<AEnemy_Buff_White>(OwnerComp.GetAIOwner()->GetPawn());
	auto Greystone = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (EnemyWhite == nullptr)
		return EBTNodeResult::Failed;

	if (EnemyWhite->IsDeath)
		return EBTNodeResult::Failed;

	if (Greystone)
	{
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(EnemyWhite->GetActorLocation(), Greystone->GetActorLocation());
		EnemyWhite->SetActorRotation(newRotation);
	}

	EnemyWhite->Attack();

	bIsAttacking = true;

	EnemyWhite->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});

	return Result;
}

void UBTTask_EnemyWhiteAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
