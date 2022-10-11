// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanAttack.h"
#include "MyAIController.h"
#include "AIController_EnemyDust.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"


UBTDecorator_CanAttack::UBTDecorator_CanAttack()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_CanAttack::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;
	
	auto Greystone = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	auto Countess = Cast<AMyCharacter_Countess>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	auto Sparrow = Cast<AMyCharacter_Sparrow>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));
	
	if (Greystone != nullptr)
		return bResult && Greystone->GetDistanceTo(CurrentPawn) <= 350.f;
	
	else if (Countess != nullptr)
		return bResult && Countess->GetDistanceTo(CurrentPawn) <= 550.f;

	else if (Sparrow != nullptr)
		return bResult && Sparrow->GetDistanceTo(CurrentPawn) <= 450.f;

	else
		return false;

	//FHitResult Hit;
	//FVector Start = CurrentPawn->GetActorLocation();
	//FVector End = Target->GetActorLocation();
	//ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;
	//GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel);
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(CurrentPawn);
	//QueryParams.AddIgnoredActor(Target);
	//GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	//if (!Hit.bBlockingHit)
	//	DrawDebugLine(GetWorld(), Start, End, FColor::Green);
	//if (Greystone == nullptr)
	//	return false;
	//return bResult && Target->GetDistanceTo(CurrentPawn) <= 200.f; //&& !Hit.bBlockingHit;

}

