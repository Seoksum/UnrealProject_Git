// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultMapLevelScriptActor.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "Enemy_Buff_White.h"
#include "MyStatComponent.h"
#include "Engine/World.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"



ADefaultMapLevelScriptActor::ADefaultMapLevelScriptActor()
{


}

void ADefaultMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> Dusks;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindEnemyDuskType, Dusks);
	for (AActor* actor : Dusks)
	{
		AEnemy_Dusk* Dusk = Cast<AEnemy_Dusk>(actor);
		Dusk->OnEnemyDeath.AddUObject(this, &ADefaultMapLevelScriptActor::CatchDusk);
	}

	TArray<AActor*> Downs;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindEnemyDownType, Downs);
	for (AActor* actor : Downs)
	{
		AEnemy_Down* Down = Cast<AEnemy_Down>(actor);
		Down->OnEnemyDeath.AddUObject(this, &ADefaultMapLevelScriptActor::CatchDown);
	}

	TArray<AActor*> Whites;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindEnemyWhiteType, Whites);
	for (AActor* actor : Whites)
	{
		AEnemy_Buff_White* White = Cast<AEnemy_Buff_White>(actor);
		White->OnEnemyDeath.AddUObject(this, &ADefaultMapLevelScriptActor::CatchWhite);
	}
}

void ADefaultMapLevelScriptActor::CatchDusk()
{
	CatchDuskCount++;

	if (CatchDuskCount == 3)
	{
		TArray<AActor*> Gates;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("SecondGate"), Gates);
		for (auto gate : Gates)
		{
			FRotator GateRotation = gate->GetActorRotation();
			GateRotation.Yaw += 100.f;
			gate->SetActorRotation(GateRotation);
		}
	}
}

void ADefaultMapLevelScriptActor::CatchDown()
{
	CatchDownCount++;

	if (CatchDownCount == 3)
	{
		OnLevelUp.Broadcast();

		TArray<AActor*> Gates;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("ThirdGate"), Gates);
		for (auto gate : Gates)
		{
			FRotator GateRotation = gate->GetActorRotation();
			GateRotation.Yaw += 100.f;
			gate->SetActorRotation(GateRotation);
		}
	}
}

void ADefaultMapLevelScriptActor::CatchWhite()
{
	CatchWhiteCount++;

	if (CatchWhiteCount == 3)
	{
		TArray<AActor*> Gates;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("FirstGate"), Gates);
		for (auto gate : Gates)
		{
			FRotator GateRotation = gate->GetActorRotation();
			GateRotation.Yaw += 100.f;
			gate->SetActorRotation(GateRotation);
		}
	}
}