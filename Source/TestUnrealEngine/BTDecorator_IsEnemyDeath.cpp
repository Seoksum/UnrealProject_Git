// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsEnemyDeath.h"
#include "MyAIController.h"
#include "AIController_EnemyDust.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "Enemy_Buff_White.h"
#include "Enemy_Boss.h"


UBTDecorator_IsEnemyDeath::UBTDecorator_IsEnemyDeath()
{
	NodeName = TEXT("IsEnemyDeath");
}

bool UBTDecorator_IsEnemyDeath::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;

	auto Enemy_Dusk = Cast<AEnemy_Dusk>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	auto Enemy_Down = Cast<AEnemy_Down>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	auto Enemy_White = Cast<AEnemy_Buff_White>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	auto Enemy_Boss = Cast<AEnemy_Boss>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	
	if (Enemy_Dusk != nullptr)
		return bResult && Enemy_Dusk->IsDeath;

	if (Enemy_Down != nullptr)
		return bResult && Enemy_Down->IsDeath;

	if (Enemy_White != nullptr)
		return bResult && Enemy_White->IsDeath;

	if (Enemy_Boss != nullptr)
		return bResult && Enemy_Boss->IsDeath;
	
	return false;
}
