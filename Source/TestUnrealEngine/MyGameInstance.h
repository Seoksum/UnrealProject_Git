// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"



UENUM()
enum ECharacterIndex
{
	Greystone,
	Countess,
	Sparrow
};

USTRUCT()
struct FMyCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_Q;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_E;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMana;


};

/**
 * 
 */



UCLASS()
class TESTUNREALENGINE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

	virtual void Init() override;

	FMyCharacterData* GetStatData(int32 Level);

	void SetCharacterTypeIndex(int32 Index);
	int32 GetCharacterTypeIndex();

	void SetMeshIndex(int32 Index);
	int32 GetMeshIndex();

	void SetHpCount(int32 HP);
	void SetManaCount(int32 MANA);
	int32 GetHpCount();
	int32 GetManaCount();


	void SetNowLevel(int32 Level);
	int32 GetNowLevel();

	void SetNowHp(int32 HP);
	int32 GetNowHp();

	void SetNowMana(int32 Mana);
	int32 GetNowMana();

	void SetPlayFirstMapSequence(bool flag);
	bool GetPlayFirstMapSequence();

	void SetRemainingCount(int32 Count);
	int32 GetRemainingCount();
	
	UPROPERTY(VisibleAnywhere)
	bool IsFirst = true;

	void SetStatComp(class UMyStatComponent* StatComp);
	UMyStatComponent* GetStatComp();

	

private:
	UPROPERTY()
	class UDataTable* MyStats;

	UPROPERTY(VisibleAnywhere)
	class AMyGameModeBase* MyGameMode;

	UPROPERTY(VisibleAnywhere)
	int32 CharacterTypeIdx;

	UPROPERTY(VisibleAnywhere)
	int32 MeshIdx = 0;

	UPROPERTY(VisibleAnywhere)
	int32 HpCount;

	UPROPERTY(VisibleAnywhere)
	int32 ManaCount;

	UPROPERTY(VisibleAnywhere)
	int32 NowHp;

	UPROPERTY(VisibleAnywhere)
	int32 NowMana;

	UPROPERTY(VisibleAnywhere)
	int32 NowLevel;

	UPROPERTY(VisibleAnywhere)
	class UMyStatComponent* Stat;

	UPROPERTY(EditAnywhere)
	bool IsPlayFirstMapSequence = false;

	UPROPERTY()
	class UMyStatComponent* CurrentStatComp;

	UPROPERTY()
	int32 RemainingCount = 3;

};
