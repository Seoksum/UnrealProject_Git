// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_EnemyWhite.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_AnimInstance.h"
#include "Enemy_Buff_White.h"



const FName AAIController_EnemyWhite::HomePosKey(TEXT("HomePos"));
const FName AAIController_EnemyWhite::PatrolPosKey(TEXT("PatrolPos"));

AAIController_EnemyWhite::AAIController_EnemyWhite()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_EnemyWhite.BT_EnemyWhite'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_EnemyWhite.BB_EnemyWhite'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
}

void AAIController_EnemyWhite::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BlackboardData, Blackboard))
	{
		AEnemy_Buff_White* EnemyWhite = Cast<AEnemy_Buff_White>(InPawn);
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsObject(FName(TEXT("Enemy")), EnemyWhite);

		if (RunBehaviorTree(BehaviorTree))
		{
			//TODO
		}
	}
}

void AAIController_EnemyWhite::OnUnPossess()
{
	Super::OnUnPossess();
}
