// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthInterface.h"
#include "Enemy_Down.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnEnemyDeath);

UCLASS()
class TESTUNREALENGINE_API AEnemy_Down : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy_Down();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


public:
	virtual void Tick(float DeltaTime) override;

	void Attack();
	void AttackCheck();

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	//void EnemyDeathMontage();

	virtual void OnDeath_Implementation() override;
	void EnemyDeath();


private:
	UPROPERTY()
	class UEnemy_AnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsAttacking = false;

	FTimerHandle DeathTimerHandle;


public:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;

	UPROPERTY(VisibleAnywhere)
	class UMyStatComponent* Stat;

	FOnAttackEnd OnAttackEnd;

	FOnEnemyDeath OnEnemyDeath;

	UPROPERTY(EditAnywhere)
	int32 Level = 3;

	UPROPERTY(VisibleAnywhere, Category = Pawn)
	bool IsDeath = false;

	UPROPERTY(EditAnywhere)
	bool PotionVal;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickup> HpPotionToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class APickup> ManaPotionToSpawn;

};
