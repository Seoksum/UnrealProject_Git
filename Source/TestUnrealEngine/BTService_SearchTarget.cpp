// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SearchTarget.h"
#include "MyAIController.h"
#include "AIController_EnemyDust.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"


UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 700.f;
	
	if (World == nullptr)
		return;


	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);

	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			AMyCharacter* Greystone = Cast<AMyCharacter>(OverlapResult.GetActor());
			AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(OverlapResult.GetActor());
			AMyCharacter_Sparrow* Sparrow = Cast<AMyCharacter_Sparrow>(OverlapResult.GetActor());
			
			if (Greystone /*&& MyCharacter->GetController()->IsPlayerController()*/)
			{				
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Greystone);
				return;
			}
			else if (Countess)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Countess);
				return;
			}

			else if (Sparrow != nullptr)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Sparrow);
				return;
			}
		}

	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")),nullptr);
	}
}
