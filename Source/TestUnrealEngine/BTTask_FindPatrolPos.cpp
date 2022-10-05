// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "MyAIController.h"
#include "AIController_EnemyDust.h"
#include "AIController_EnemyWhite.h"
#include "AIController_EnemyBoss.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"



UBTTask_FindPatrolPos::UBTTask_FindPatrolPos()
{
	NodeName = TEXT("FindPatrolPos");
}

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr)
		return EBTNodeResult::Failed;
	
	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return EBTNodeResult::Failed;
	

	FVector Origin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AMyAIController::HomePosKey);
	FVector DownOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIController_EnemyDust::HomePosKey);
	FVector BuffWhiteOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIController_EnemyWhite::HomePosKey);
	FVector BossOrigin = OwnerComp.GetBlackboardComponent()->GetValueAsVector(AAIController_EnemyBoss::HomePosKey);

	FNavLocation RandomLocation;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, 500.f, RandomLocation))
	{
		//UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, RandomLocation);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AMyAIController::PatrolPosKey, RandomLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIController_EnemyDust::PatrolPosKey, RandomLocation.Location);

		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIController_EnemyWhite::PatrolPosKey, RandomLocation.Location);
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(AAIController_EnemyBoss::PatrolPosKey, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}


	return EBTNodeResult::Failed;
}


