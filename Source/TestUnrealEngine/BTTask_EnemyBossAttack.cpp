// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_EnemyBossAttack.h"
#include "AIController_EnemyBoss.h"
#include "Enemy_Boss.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_EnemyBossAttack::UBTTask_EnemyBossAttack()
{

}


EBTNodeResult::Type UBTTask_EnemyBossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto EnemyBoss = Cast<AEnemy_Boss>(OwnerComp.GetAIOwner()->GetPawn());
	auto Greystone = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	
	if (EnemyBoss == nullptr)
		return EBTNodeResult::Failed;

	if(EnemyBoss->IsDeath)
		return EBTNodeResult::Failed;

	if (Greystone)
	{
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(EnemyBoss->GetActorLocation(), Greystone->GetActorLocation());
		EnemyBoss->SetActorRotation(newRotation);
	}

	EnemyBoss->Attack();
	bIsAttacking = true;
	EnemyBoss->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});
	
	return Result;
}

void UBTTask_EnemyBossAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
