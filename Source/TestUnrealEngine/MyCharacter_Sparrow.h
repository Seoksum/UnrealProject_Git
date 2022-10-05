// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthInterface.h"
#include "Interactable.h"
#include "Pickup.h"
#include "MyCharacter_Sparrow.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE(FOnArrowSpeedChanged);

UCLASS()
class TESTUNREALENGINE_API AMyCharacter_Sparrow : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter_Sparrow();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Landed(const FHitResult& Hit) override;

public:
	// Called every frame
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

	void ArrowZoomIn();
	void ArrowZoomOut();

	void Attack();
	void Attack_Q();
	void Attack_E();
	void Attack_R();

	void EndAttackingQ();
	void EndAttackingE();
	void EndAttackingR();

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void RefreshUI();

	void CatchDusk();
	void CatchDown();
	void CatchWhite();

	virtual void OnDeath_Implementation() override;

	UFUNCTION()
	void DoubleJump();

	void ToggleInventory(); // I 키를 누르면 InventoryHUD <-> IngameHUD 바꿔주는 함수

	void Interact(); // Z키를 눌러 아이템 수집

	void CheckForInteractable(APickup* PickItem);//라인트레이스를 이용해서 플레이어 앞에 있는 액터가 interactable가능한 아이템인지 확인해주는 함수

	void ThrowArrow(); // 속도 조절 x 발사 함수
	void ThrowArrowSpeed();// 속도 조절 O 발사 함수
	
	void PressClimbingUp(); // 1번 키를 눌러 캐릭터 벽 타기
	void ReleaseClimbingUp();

	void PressClimbingComplete(); // 2번 키를 눌러 캐릭터 착지
	void ReleaseClimbingComplete();
	
	void PlayerRespawn();

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY()
		class USparrowAnimInstance* AnimInstance;

	UPROPERTY()
	int32 Mana = 5;

	UPROPERTY()
	int32 SparrowLevel = 4;

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

	FTimerHandle ClimbingHandle;
	FTimerHandle ClimbingDownHandle;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

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
	bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingQ = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingE = false;

	UPROPERTY(VisibleAnywhere)
	bool IsAttackingR = false;

	UPROPERTY(VisibleAnywhere)
	class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* LevelUpEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* LevelUpSound;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class APortalToSecondMap> FindPortalToSecond;

	FOnAttackEnd OnAttackEnd;
	FOnArrowSpeedChanged OnArrowSpeedChanged;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWorld* MyWorld;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Arrow)
	TSubclassOf<class AWeapon_Arrow> ArrowClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystem* AttackEffect;

	UPROPERTY(VisibleAnywhere)
	class AMyGameModeBase* MyGameMode;

	UPROPERTY(VisibleAnywhere)
	class AMyPlayerController* MyPlayerController;

	AInteractable* CurrentInteractable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> ArrowHUDClass;

	UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(APickup* Item);

	UPROPERTY(EditAnywhere)
	TArray<APickup*> Inventory;

	UPROPERTY(EditAnywhere)
	int32 HpCount = 0;

	UPROPERTY(EditAnywhere)
	int32 ManaCount = 0;

	UPROPERTY(EditAnywhere)
	bool AttackRMove = false;

	UPROPERTY(VisibleAnywhere)
	bool bIsOnWall = false;

	UPROPERTY(VisibleAnywhere)
	bool IsClimbingUp = false;

	UPROPERTY(VisibleAnywhere)
	bool IsClimbingComplete = false;

	UPROPERTY(VisibleAnywhere)
	bool IsFallingNow = false;

	UPROPERTY(VisibleAnywhere)
	int32 ClimbCount = 0;

	UPROPERTY(VisibleAnywhere)
	class ASquare* Wall;

	UPROPERTY(VisibleAnywhere)
	bool IsRightMouse = false;

	UPROPERTY(VisibleAnywhere)
	float ArrowSpeedVal = 400.f;

	UPROPERTY()
	class UArrowWidget* MyArrowWidget;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UArrowWidget> BP_ArrowWidget;
};
