// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnAttackHit_Q);
DECLARE_MULTICAST_DELEGATE(FOnAttackHit_E);
DECLARE_MULTICAST_DELEGATE(FOnAttackHit_R);
/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void PlayAttackMontage();
	void PlayAttack_Q();
	void PlayAttack_E();
	void PlayAttack_R();

	
	void JumpToSection(int32 SectionIndex);
	FName GetAttackMontageName(int32 SectionIndex);
	
	

private:
	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_AttackHit_Q();

	UFUNCTION()
	void AnimNotify_AttackHit_E();

	UFUNCTION()
	void AnimNotify_AttackHit_R();


public:

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category=Pawn,Meta=(AllowPrivateAccess=true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsDoubleJump;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Vertical;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_Q;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_E;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Attack_R;



public:
	FOnAttackHit OnAttackHit;
	FOnAttackHit_Q OnAttackHit_Q;
	FOnAttackHit_E OnAttackHit_E;
	FOnAttackHit_R OnAttackHit_R;


};
