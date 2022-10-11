// Fill out your copyright notice in the Description page of Project Settings.


#include "CountessAnimInstance.h"
#include "MyCharacter_Countess.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


UCountessAnimInstance::UCountessAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Countess/DefaultAttack.DefaultAttack'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AQ(TEXT("AnimMontage'/Game/Animations/Countess/Attack_Q.Attack_Q'"));
	if (AQ.Succeeded())
	{
		Attack_Q = AQ.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AE(TEXT("AnimMontage'/Game/Animations/Countess/Attack_E.Attack_E'"));
	if (AE.Succeeded())
	{
		Attack_E = AE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AR(TEXT("AnimMontage'/Game/Animations/Countess/Attack_R.Attack_R'"));
	if (AR.Succeeded())
	{
		Attack_R = AR.Object;
	}

}

void UCountessAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner(); //나를 포함하고 있는 Pawn을 Get하겠다. 
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto PlayerCharacter = Cast<AMyCharacter_Countess>(Pawn);
		if (PlayerCharacter)
		{
			IsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsDoubleJump = PlayerCharacter->IsDoubleJumping;

			Vertical = PlayerCharacter->UpDownValue;
			Horizontal = PlayerCharacter->LeftRightValue;
		}
	}
}

void UCountessAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UCountessAnimInstance::PlayAttack_Q()
{
	Montage_Play(Attack_Q, 1.f);
}

void UCountessAnimInstance::PlayAttack_E()
{
	Montage_Play(Attack_E, 1.f);

}

void UCountessAnimInstance::PlayAttack_R()
{
	Montage_Play(Attack_R, 1.f);
}

void UCountessAnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UCountessAnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UCountessAnimInstance::AnimNotify_AttackHit()
{
	// 사운드 재생, 패킷전송, 피격?판정등을 하는 함수
	//UE_LOG(LogTemp, Log, TEXT("AnimNotify_AttackHit"));
	OnAttackHit.Broadcast();
}

void UCountessAnimInstance::AnimNotify_AttackHit_Q()
{
	OnAttackHit_Q.Broadcast();
}

void UCountessAnimInstance::AnimNotify_AttackHit_E()
{
	OnAttackHit_E.Broadcast();
}

void UCountessAnimInstance::AnimNotify_AttackHit_R()
{
	OnAttackHit_R.Broadcast();
}


