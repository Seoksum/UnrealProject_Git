// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy_AnimInstance.h"
#include "MyAIController.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "Enemy_Buff_White.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Enemy_Boss.h"



UEnemy_AnimInstance::UEnemy_AnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Enemies/Montage/Enemy_Dusk_AttackMontage.Enemy_Dusk_AttackMontage'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM(TEXT("AnimMontage'/Game/Enemies/Montage/Enemy_Dusk_DeathMontage.Enemy_Dusk_DeathMontage'"));
	if (DM.Succeeded())
	{
		DeathMontage = DM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Down(TEXT("AnimMontage'/Game/Enemies/Montage/Enemy_Down_AttackMontage.Enemy_Down_AttackMontage'"));
	if (AM_Down.Succeeded())
	{
		Down_AttackMontage = AM_Down.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM_Down(TEXT("AnimMontage'/Game/Enemies/Montage/Enemy_Down_DeathMontage.Enemy_Down_DeathMontage'"));
	if (DM_Down.Succeeded())
		Down_DeathMontage = DM_Down.Object;
	



	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_White(TEXT("AnimMontage'/Game/Enemies/Montage/BuffWhite_AttackMontage.BuffWhite_AttackMontage'"));
	if (AM_White.Succeeded())
		White_AttackMontage = AM_White.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DM_White(TEXT("AnimMontage'/Game/Enemies/Montage/BuffWhite_DeathMontage.BuffWhite_DeathMontage'"));
	if (DM_White.Succeeded())
		White_DeathMontage = DM_White.Object;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OM_White(TEXT("AnimMontage'/Game/Enemies/Buff_White/OnAttackedMontage.OnAttackedMontage'"));
	if (OM_White.Succeeded())
		White_OnAttackedMontage = OM_White.Object;



	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Boss(TEXT("AnimMontage'/Game/Enemies/Montage/Enemy_Boss_AttackMontage.Enemy_Boss_AttackMontage'"));
	if (AM_Boss.Succeeded())
		Boss_AttackMontage = AM_Boss.Object;
	
}

void UEnemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	Dusk_BSNode();
	Down_BSNode();
	Buff_White_BSNode();
	Boss_BSNode();
}

void UEnemy_AnimInstance::Dusk_BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy_Dusk>(Pawn);
		if (Enemy)
		{
			SpeedSide = Enemy->GetActorRightVector().Size();
			SpeedForward = Enemy->GetActorForwardVector().Size();
		}
	}
}

void UEnemy_AnimInstance::Down_BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Down_Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy_Down>(Pawn);
		if (Enemy)
		{
			Down_SpeedSide = Enemy->GetActorRightVector().Size();
			Down_SpeedForward = Enemy->GetActorForwardVector().Size();
		}
	}
}

void UEnemy_AnimInstance::Buff_White_BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		White_Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy_Buff_White>(Pawn);
		if (Enemy)
		{
			White_SpeedSide = Enemy->GetActorRightVector().Size();
			White_SpeedForward = Enemy->GetActorForwardVector().Size();
		}
	}
}

void UEnemy_AnimInstance::Boss_BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Boss_Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy_Boss>(Pawn);
		if (Enemy)
		{
			Boss_SpeedSide = Enemy->GetActorRightVector().Size();
			Boss_SpeedForward = Enemy->GetActorForwardVector().Size();
		}
	}
}

void UEnemy_AnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 2.f);
}

void UEnemy_AnimInstance::PlayDeathMontage()
{
	Montage_Play(DeathMontage, 1.f);
}

void UEnemy_AnimInstance::PlayAttackMontage_Down()
{
	Montage_Play(Down_AttackMontage, 2.f);
}

void UEnemy_AnimInstance::PlayDeathMontage_Down()
{
	Montage_Play(Down_DeathMontage, 1.f);
}

void UEnemy_AnimInstance::PlayAttackMontage_White()
{
	Montage_Play(White_AttackMontage, 2.f);
}

void UEnemy_AnimInstance::PlayDeathMontage_White()
{
	Montage_Play(White_DeathMontage, 0.7f);
}

void UEnemy_AnimInstance::PlayAttackedMontage_White()
{
	Montage_Play(White_OnAttackedMontage, 1.f);
}

void UEnemy_AnimInstance::PlayAttackMontage_Boss()
{
	Montage_Play(Boss_AttackMontage, 1.f);
}

void UEnemy_AnimInstance::PlayDeathMontage_Boss()
{
	
}

void UEnemy_AnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, Boss_AttackMontage);
}

FName UEnemy_AnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UEnemy_AnimInstance::AnimNotify_AttackHit()
{
	OnDuskAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_AttackHitDown()
{
	OnDownAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_AttackHitWhite()
{
	OnWhiteAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_BossAttackHit()
{
	OnBossAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_BossSphereHit()
{
	OnBossSphereHit.Broadcast();
}
