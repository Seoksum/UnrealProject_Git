// Fill out your copyright notice in the Description page of Project Settings.


#include "SparrowAnimInstance.h"
#include "MyCharacter_Sparrow.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"


USparrowAnimInstance::USparrowAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/Animations/Sparrow/DefaultAttack.DefaultAttack'"));
	if (AM.Succeeded())
		AttackMontage = AM.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AQ(TEXT("AnimMontage'/Game/Animations/Sparrow/Attack_Q.Attack_Q'"));
	if (AQ.Succeeded())
		Attack_Q = AQ.Object;
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AE(TEXT("AnimMontage'/Game/Animations/Sparrow/Attack_E.Attack_E'"));
	if (AE.Succeeded())
		Attack_E = AE.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AR(TEXT("AnimMontage'/Game/Animations/Sparrow/Attack_R.Attack_R'"));
	if (AR.Succeeded())
		Attack_R = AR.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CM(TEXT("AnimMontage'/Game/Animations/Sparrow/ClimbingMontage.ClimbingMontage'"));
	if (CM.Succeeded())
		ClimbingComplete = CM.Object;
}

void USparrowAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner(); //나를 포함하고 있는 Pawn을 Get하겠다. 
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto PlayerCharacter = Cast<AMyCharacter_Sparrow>(Pawn);
		if (PlayerCharacter)
		{
			Vertical = PlayerCharacter->UpDownValue;
			Horizontal = PlayerCharacter->LeftRightValue;

			IsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsDoubleJump = PlayerCharacter->IsDoubleJumping;

			IsOnWall = PlayerCharacter->bIsOnWall;
			IsClimbingUp = PlayerCharacter->IsClimbingUp;
			
			
		}
	}
}

void USparrowAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void USparrowAnimInstance::PlayAttack_Q()
{
	Montage_Play(Attack_Q, 1.f);

}

void USparrowAnimInstance::PlayAttack_E()
{
	Montage_Play(Attack_E, 1.f);
}

void USparrowAnimInstance::PlayAttack_R()
{
	Montage_Play(Attack_R, 1.f);
}

void USparrowAnimInstance::PlayClimbingComplete()
{
	Montage_Play(ClimbingComplete, 1.f);
}




