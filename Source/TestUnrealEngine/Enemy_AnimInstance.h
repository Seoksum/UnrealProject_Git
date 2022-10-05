// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_AnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnDuskAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnDownAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnWhiteAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnBossAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnBossSphereHit);

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UEnemy_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UEnemy_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void Dusk_BSNode();
	void Down_BSNode();
	void Buff_White_BSNode();
	void Boss_BSNode();

	void PlayAttackMontage();
	void PlayDeathMontage();

	void PlayAttackMontage_Down();
	void PlayDeathMontage_Down();

	void PlayAttackMontage_White();
	void PlayDeathMontage_White();
	void PlayAttackedMontage_White();

	void PlayAttackMontage_Boss();
	void PlayDeathMontage_Boss();
	
	void JumpToSection(int32 SectionIndex);
	FName GetAttackMontageName(int32 SectionIndex);


private:
	UFUNCTION()
	void AnimNotify_AttackHit();

	UFUNCTION()
	void AnimNotify_AttackHitDown();

	UFUNCTION()
	void AnimNotify_AttackHitWhite();

	UFUNCTION()
	void AnimNotify_BossAttackHit();

	UFUNCTION()
	void AnimNotify_BossSphereHit();
	
// Dusk 변수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* DeathMontage;

// Down 변수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Down_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Down_SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Down_SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Down_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Down_DeathMontage;


// Buff_White 변수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float White_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float White_SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float White_SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* White_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* White_DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* White_OnAttackedMontage;

// Boss 변수
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Boss_Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Boss_SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	float Boss_SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Boss_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	UAnimMontage* Boss_DeathMontage;



public:
	FOnDuskAttackHit OnDuskAttackHit;
	FOnDownAttackHit OnDownAttackHit;
	FOnWhiteAttackHit OnWhiteAttackHit;
	FOnBossAttackHit OnBossAttackHit;
	FOnBossSphereHit OnBossSphereHit;
};
