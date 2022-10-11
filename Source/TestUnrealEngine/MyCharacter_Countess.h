// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthInterface.h"
#include "Interactable.h"
#include "Pickup.h"
#include "MyCharacter_Countess.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class TESTUNREALENGINE_API AMyCharacter_Countess : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter_Countess();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Landed(const FHitResult& Hit) override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);
	void CameraZoom(const float Value);
	
	void Sprint();
	void StopSprinting();

	void Attack();
	void Attack_Q();
	void Attack_E();
	void Attack_R();
	
	FHitResult AttackCollisionCheck();
	TArray<FHitResult> AttackE_CollisionCheck();
	TArray<FHitResult> AttackR_CollisionCheck();

	void AttackCheck();
	void AttackCheck_Q();
	void AttackCheck_E();
	void AttackCheck_R();

	void EndAttackingQ();
	void EndAttackingE();
	void EndAttackingR();


	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void LevelUp();

	virtual void OnDeath_Implementation() override;

	UFUNCTION()
	void DoubleJump();

	void ToggleInventory();

	void Interact();

	//라인트레이스를 이용해서 플레이어 앞에 있는 액터가 interactable가능한 아이템인지 확인해주는 함수
	void CheckForInteractable(APickup* PickItem);

	void PlayerRespawn();

private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingQ = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingE = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingR = false;

	UPROPERTY()
	class UCountessAnimInstance* AnimInstance;

	UPROPERTY()
	int32 AttackIndex = 0;

	UPROPERTY()
	int32 Mana = 5;

	UPROPERTY()
	int32 CountessLevel = 4;

	UPROPERTY()
	int32 Remaining_SkillQ = 3;

	UPROPERTY()
	int32 Remaining_SkillE = 4;

	UPROPERTY()
	int32 Remaining_SkillR = 5;

	UPROPERTY()
	bool bIsSprinting;

	UPROPERTY()
	int32 DoubleJumpCounter;

	FTimerHandle QSkillHandle;
	FTimerHandle ESkillHandle;
	FTimerHandle RSkillHandle;

	UPROPERTY()
	class ADefaultMapLevelScriptActor* DefaultScriptActor;

public:
	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	UPROPERTY()
	float UpDownValue = 0;

	UPROPERTY()
	float LeftRightValue = 0;

	UPROPERTY()
	bool IsDoubleJumping;

	UPROPERTY(VisibleAnywhere)
	class UMyStatComponent* Stat;

	//UPROPERTY(VisibleAnywhere)
	//class UFootIKComponent* IKFootPlacement;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* AttackEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* AttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* LevelUpEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* LevelUpSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APortalToSecondMap> FindPortalToSecond;

	FOnAttackEnd OnAttackEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWorld* MyWorld;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//class UMyHUD* MyCurrentHUD;

	UPROPERTY(VisibleAnywhere)
	class AMyGameModeBase* MyGameMode;

	UPROPERTY(VisibleAnywhere)
	class AMyPlayerController* MyPlayerController;

//////////////////////////////////////////////

	AInteractable* CurrentInteractable;


	// 아이템을 인벤토리에 추가
	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(APickup* Item);

	// 플레이어의 인벤토리, pickup 물건을 배열로 나타낸다
	UPROPERTY(EditAnywhere)
		TArray<APickup*> Inventory;


	UPROPERTY(EditAnywhere)
	int32 HpCount = 0;

	UPROPERTY(EditAnywhere)
	int32 ManaCount = 0;

	UPROPERTY(EditAnywhere)
	bool AttackQMove = false;


};
