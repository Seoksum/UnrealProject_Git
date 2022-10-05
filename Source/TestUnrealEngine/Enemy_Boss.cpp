// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_Boss.h"
#include "Enemy_AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "TimerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyStatComponent.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"
#include "EnemyDust_Widget.h"
#include "Pickup.h"
#include "AIController_EnemyBoss.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "Kismet/GameplayStatics.h"
#include "BossWeapon_Sphere.h"


// Sets default values
AEnemy_Boss::AEnemy_Boss()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIController_EnemyBoss::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonBoris/Characters/Heroes/Boris/Meshes/Boris.Boris'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("Stat"));
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 270.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_EnemyBuffWhite.WBP_EnemyBuffWhite_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	Level = 5;

	FStringAssetReference SequenceName(TEXT("LevelSequence'/Game/Cinematics/BossDeathSequence.BossDeathSequence'"));
	SeqeunceDeath = Cast<ULevelSequence>(SequenceName.TryLoad());

}

// Called when the game starts or when spawned
void AEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();
	
	Stat->SetLevel(Level);
}

void AEnemy_Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UEnemy_AnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy_Boss::OnAttackMontageEnded);
		AnimInstance->OnBossAttackHit.AddUObject(this, &AEnemy_Boss::AttackCheck);
		AnimInstance->OnBossSphereHit.AddUObject(this, &AEnemy_Boss::FallMySphere);
	}

	HpBar->InitWidget();

	auto EnemyWidget = Cast<UEnemyDust_Widget>(HpBar->GetUserWidgetObject());
	if (EnemyWidget)
	{
		EnemyWidget->BindHp(Stat);
		EnemyWidget->BindLevel(Level);
	}
	
}

void AEnemy_Boss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (GetDistanceTo(Player) <= 1000.f)
		HpBar->SetVisibility(true);
	else
		HpBar->SetVisibility(false);

}

void AEnemy_Boss::Attack()
{
	if (IsAttacking || Stat->GetEnemyHp() <= 0.f)
		return;

	AnimInstance->PlayAttackMontage_Boss();
	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 4;
	IsAttacking = true;
}

void AEnemy_Boss::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 400.f;
	float AttackRadius = 200.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity, // 회전각X
		ECollisionChannel::ECC_EngineTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);

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

void AEnemy_Boss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy_Boss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	
	Stat->OnEnemyAttacked(DamageAmount);

	FVector location = GetActorLocation();
	FVector AttackerLocation = DamageCauser->GetActorLocation();

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(location, AttackerLocation);
	//시작 위치의 개체(나)가 대상 위치(적)를 가리키도록 회전을 찾습니다.
	SetActorRotation(rotator);

	return DamageAmount;
}

void AEnemy_Boss::OnDeath_Implementation()
{
	if (SeqeunceDeath)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* currentLevelSequenceActor = nullptr;

		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			SeqeunceDeath,
			FMovieSceneSequencePlaybackSettings(),
			currentLevelSequenceActor);

		SequencePlayer->Play();
	}

	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy_Boss::EnemyDeath, 2.f, false, 2.f);
	
}

void AEnemy_Boss::EnemyDeath()
{
	Destroy();

	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}

void AEnemy_Boss::FallMySphere()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector PlayerLocation = Player->GetActorLocation();
	PlayerLocation.Z += 500.f;
	FVector SpawnLocation = PlayerLocation;
	FRotator Rotation = FRotator::ZeroRotator;
	

	ABossWeapon_Sphere* BossSphere = GetWorld()->SpawnActor<ABossWeapon_Sphere>(SphereClass, SpawnLocation,Rotation,SpawnParams);



}




