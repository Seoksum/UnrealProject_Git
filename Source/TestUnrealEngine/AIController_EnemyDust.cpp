// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_EnemyDust.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_Down.h"
#include "Enemy_AnimInstance.h"


const FName AAIController_EnemyDust::HomePosKey(TEXT("HomePos"));
const FName AAIController_EnemyDust::PatrolPosKey(TEXT("PatrolPos"));

AAIController_EnemyDust::AAIController_EnemyDust()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_EnemyDown.BT_EnemyDown'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_EnemyDown.BB_EnemyDown'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
	
}


void AAIController_EnemyDust::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		AEnemy_Down* EnemyDown = Cast<AEnemy_Down>(InPawn);
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsObject(FName(TEXT("Enemy")), EnemyDown);
		if (RunBehaviorTree(BehaviorTree))
		{
			// TODO
		}
	}
}

void AAIController_EnemyDust::OnUnPossess()
{
	Super::OnUnPossess();

	
}
