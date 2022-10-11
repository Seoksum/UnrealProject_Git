// Fill out your copyright notice in the Description page of Project Settings.


#include "AIController_EnemyBoss.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_Boss.h"


const FName AAIController_EnemyBoss::HomePosKey(TEXT("HomePos"));
const FName AAIController_EnemyBoss::PatrolPosKey(TEXT("PatrolPos"));


AAIController_EnemyBoss::AAIController_EnemyBoss()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_EnemyBoss.BT_EnemyBoss'"));
	if (BT.Succeeded())
		BehaviorTree = BT.Object;

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_EnemyBoss.BB_EnemyBoss'"));
	if (BD.Succeeded())
		BlackboardData = BD.Object;
}

void AAIController_EnemyBoss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		AEnemy_Boss* EnemyBoss = Cast<AEnemy_Boss>(InPawn);
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsObject(FName(TEXT("Enemy")), EnemyBoss);
		if (RunBehaviorTree(BehaviorTree))
		{
			// TODO
		}
	}

}

void AAIController_EnemyBoss::OnUnPossess()
{
	Super::OnUnPossess();


}