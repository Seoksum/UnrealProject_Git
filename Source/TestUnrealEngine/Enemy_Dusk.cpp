// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Dusk.h"
#include "MyAIController.h"
#include "Enemy_AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "EnemyDust_Widget.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyStatComponent.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"
#include "Pickup.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AEnemy_Dusk::AEnemy_Dusk()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonMinions/Characters/Buff/Buff_Red/Meshes/Buff_Red.Buff_Red'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));
	

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 240.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_EnemyBuffWhite.WBP_EnemyBuffWhite_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	Level = 2;
}

// Called when the game starts or when spawned
void AEnemy_Dusk::BeginPlay()
{
	Super::BeginPlay();

	Stat->SetLevel(Level);
}

void AEnemy_Dusk::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	AnimInstance = Cast<UEnemy_AnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy_Dusk::OnAttackMontageEnded);
		AnimInstance->OnDuskAttackHit.AddUObject(this, &AEnemy_Dusk::AttackCheck);

	}

	HpBar->InitWidget();

	// TODO , HpBar 업데이트 부분(델리게이트, 바인딩)
	auto EnemyWidget = Cast<UEnemyDust_Widget>(HpBar->GetUserWidgetObject());
	if (EnemyWidget)
	{
		EnemyWidget->BindHp(Stat);
		EnemyWidget->BindLevel(Level);
	}

}

void AEnemy_Dusk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (GetDistanceTo(Player) <= 1500.f)
		HpBar->SetVisibility(true);
	else
		HpBar->SetVisibility(false);
}

void AEnemy_Dusk::Attack()
{
	if (IsAttacking || Stat->GetEnemyHp() <= 0.f)
		return;

	AnimInstance->PlayAttackMontage();
	//Stat->OnAttacking(2.f);

	IsAttacking = true;
}

void AEnemy_Dusk::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 50.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector()*AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	AMyCharacter* Greystone = Cast<AMyCharacter>(HitResult.Actor);
	AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(HitResult.Actor);
	AMyCharacter_Sparrow* Sparrow = Cast<AMyCharacter_Sparrow>(HitResult.Actor);
	if (bResult)
	{
		if (Greystone)
		{
			FDamageEvent DamageEvent;
			Greystone->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
		}
		else if (Countess)
		{
			FDamageEvent DamageEvent;
			Countess->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
		}
		else if (Sparrow)
		{
			FDamageEvent DamageEvent;
			Sparrow->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
		}
	}
}


void AEnemy_Dusk::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy_Dusk::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	Stat->OnEnemyAttacked(DamageAmount);

	FVector location = GetActorLocation();
	FVector AttackerLocation = DamageCauser->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(location, AttackerLocation);
	SetActorRotation(rotator);
	
	return DamageAmount;
}


void AEnemy_Dusk::OnDeath_Implementation()
{
	AnimInstance->PlayDeathMontage();

	if (!IsDeath)
	{
		OnEnemyDeath.Broadcast();
		IsDeath = true;
	}

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy_Dusk::EnemyDeath, 1.f, false, 2.f);
}

void AEnemy_Dusk::EnemyDeath()
{
	Destroy();
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = GetActorLocation();

	if (PotionVal)
		GetWorld()->SpawnActor<APickup>(HpPotionToSpawn, spawnLocation, rotator, spawnParams);
	else
		GetWorld()->SpawnActor<APickup>(ManaPotionToSpawn, spawnLocation, rotator, spawnParams);

	GetWorldTimerManager().ClearTimer(DeathTimerHandle);

}

