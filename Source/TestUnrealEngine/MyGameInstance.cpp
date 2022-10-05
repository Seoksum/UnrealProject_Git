// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "CharacterSelectController.h"
#include "Kismet/GameplayStatics.h"
#include "MyStatComponent.h"



UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	MyStats = DATA.Object;

	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	NowLevel = 4;
	NowHp = GetStatData(4)->MaxHp;
	NowMana = GetStatData(4)->MaxMana;
}

void UMyGameInstance::Init()
{
	Super::Init();
	
}

FMyCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStats->FindRow<FMyCharacterData>(*FString::FromInt(Level), TEXT(""));
}

void UMyGameInstance::SetCharacterTypeIndex(int32 Index)
{
	CharacterTypeIdx = Index;
}

int32 UMyGameInstance::GetCharacterTypeIndex()
{
	return CharacterTypeIdx;
}

void UMyGameInstance::SetMeshIndex(int32 Index) 
{ 
	MeshIdx = Index; 
}

int32 UMyGameInstance::GetMeshIndex() 
{ 
	return MeshIdx; 
}


void UMyGameInstance::SetHpCount(int32 HP)
{
	HpCount = HP;
	if (HpCount < 0)
		return;
}

void UMyGameInstance::SetManaCount(int32 MANA)
{
	ManaCount = MANA;
	if (ManaCount < 0)
		return;
}

int32 UMyGameInstance::GetHpCount()
{
	return HpCount;
}

int32 UMyGameInstance::GetManaCount()
{
	return ManaCount;
}

void UMyGameInstance::SetNowLevel(int32 Level)
{
	NowLevel = Level;
	IsFirst = true;
}

int32 UMyGameInstance::GetNowLevel()
{
	return NowLevel;
}

void UMyGameInstance::SetNowHp(int32 HP)
{
	NowHp = HP;
}

int32 UMyGameInstance::GetNowHp()
{
	return NowHp;
}

void UMyGameInstance::SetNowMana(int32 Mana)
{
	NowMana = Mana;
}

int32 UMyGameInstance::GetNowMana()
{
	return NowMana;
}

void UMyGameInstance::SetPlayFirstMapSequence(bool flag)
{
	IsPlayFirstMapSequence = flag;
}

bool UMyGameInstance::GetPlayFirstMapSequence()
{
	return IsPlayFirstMapSequence;
}

void UMyGameInstance::SetRemainingCount(int32 Count)
{
	RemainingCount = Count;
}

int32 UMyGameInstance::GetRemainingCount()
{
	return RemainingCount;
}

void UMyGameInstance::SetStatComp(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
}

UMyStatComponent* UMyGameInstance::GetStatComp()
{
	return CurrentStatComp;
}


