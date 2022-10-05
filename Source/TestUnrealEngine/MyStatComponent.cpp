// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStatComponent.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HealthInterface.h"
#include "GameFramework/Actor.h"
#include "Enemy_Buff_White.h"
#include "MyCharacter.h"


// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
	Level =1;

}

void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

}
void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//SetLevel(Level);
}

void UMyStatComponent::SetLevel(int32 NewLevel)
{
	//MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);

		if (StatData)
		{
			Level = StatData->Level;
			SetEnemyHp(StatData->MaxHp);
			EnemyMaxHp = StatData->MaxHp;
			EnemyAttack = StatData->Attack;

			//UE_LOG(LogTemp, Warning, TEXT("Now Hp : %d"), Hp);
		}
	}
}

void UMyStatComponent::SetPlayerLevel(int32 NewLevel)
{
	//MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);

		if (MyGameInstance->IsFirst)
		{
			Hp = StatData->MaxHp;
			Mana = StatData->MaxMana;
			MyGameInstance->SetNowHp(Hp);
			MyGameInstance->SetNowMana(Mana);
			MyGameInstance->IsFirst = false;
		}
		else
		{
			Hp = MyGameInstance->GetNowHp();
			Mana = MyGameInstance->GetNowMana();
		}

		if (StatData)
		{
			Level = StatData->Level;
			
			SetHp(Hp);
			SetMana(Mana);

			MaxHp = StatData->MaxHp;
			MaxMana = StatData->MaxMana;

			Attack = StatData->Attack;
			Attack_Q = StatData->Attack_Q;
			Attack_E = StatData->Attack_E;
			Attack_R = StatData->Attack_R;

		}

	}
}

void UMyStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	//MyGameInstance->SetNowHp(Hp);

	if (Hp < 0)
	{
		Hp = 0;

		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}

	}
	OnHpChanged.Broadcast();
}

void UMyStatComponent::SetMana(int32 NewMana)
{
	Mana = NewMana;
	if (Mana < 0)
	{
		Mana = 0;
	}
	OnManaChanged.Broadcast();
}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	int NewHp = Hp -= DamageAmount;
	MyGameInstance->SetNowHp(NewHp);
	SetHp(NewHp);

}

void UMyStatComponent::OnAttacking(float ManaAmount)
{
	int NewMana = Mana -= ManaAmount;
	MyGameInstance->SetNowHp(NewMana);
	SetMana(NewMana);
}

void UMyStatComponent::GetHpPotion(float Amount)
{
	int NewHp = Hp += Amount;
	SetHp(NewHp);
}

void UMyStatComponent::SetEnemyHp(int32 NewHp)
{
	EnemyHp = NewHp;

	if (EnemyHp < 0)
	{
		EnemyHp = 0;

		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}
	}
	OnHpChanged.Broadcast();
}

void UMyStatComponent::OnEnemyAttacked(float DamageAmount)
{
	int NewEnemyHp = EnemyHp -= DamageAmount;
	SetEnemyHp(NewEnemyHp);
	UE_LOG(LogTemp, Warning, TEXT("Enemy Hp : %d"), NewEnemyHp);
}



