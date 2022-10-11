// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter_Countess.h"
#include "CountessAnimInstance.h"
#include "MyGameModeBase.h"
#include "MyStatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "MyHUD.h"
#include "MyCharacterrWidget.h"
#include "Components/TextBlock.h"
#include "FootIKComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"
#include "Components/Image.h"
#include "PortalToSecondMap.h"
#include "Enemy_Buff_White.h"
#include "MyPlayerController.h"
#include "Pickup.h"
#include "MyInventoryHUD.h"
#include "MyGameInstance.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "DefaultMapLevelScriptActor.h"


// Sets default values
AMyCharacter_Countess::AMyCharacter_Countess()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->JumpZVelocity = 500.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	GetMesh()->SetRelativeLocationAndRotation(
		FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	bIsSprinting = false;


	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	MyWorld = GetWorld();

	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(this, 0));

	if (MyPlayerController)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
		//UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->SetCountess();
		}
	}
}

// Called when the game starts or when spawned
void AMyCharacter_Countess::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

	DefaultScriptActor = Cast<ADefaultMapLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (DefaultScriptActor)
	{
		DefaultScriptActor->OnLevelUp.AddUObject(this, &AMyCharacter_Countess::LevelUp);
	}
}

void AMyCharacter_Countess::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MyWorld)
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetStatComp(Stat);
		CountessLevel = MyGameInstanceRef->GetNowLevel();
		Stat->SetPlayerLevel(CountessLevel);
	}

	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	AnimInstance = Cast<UCountessAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter_Countess::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter_Countess::AttackCheck);
		AnimInstance->OnAttackHit_Q.AddUObject(this, &AMyCharacter_Countess::AttackCheck_Q);
		AnimInstance->OnAttackHit_E.AddUObject(this, &AMyCharacter_Countess::AttackCheck_E);
		AnimInstance->OnAttackHit_R.AddUObject(this, &AMyCharacter_Countess::AttackCheck_R);
	}

	HpBar->InitWidget();

	UMyCharacterrWidget* HpWidget = Cast<UMyCharacterrWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(Stat);
		HpWidget->BindMana(Stat);
	}

}


void AMyCharacter_Countess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (AttackQMove)
	{
		FVector NewLocation1 = GetActorLocation() + GetActorForwardVector() * 7.f;
		SetActorLocation(NewLocation1);
	}

}

// Called to bind functionality to input
void AMyCharacter_Countess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::DoubleJump);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack_Q"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Attack_Q);
	PlayerInputComponent->BindAction(TEXT("Attack_E"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Attack_E);
	PlayerInputComponent->BindAction(TEXT("Attack_R"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Attack_R);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyCharacter_Countess::StopSprinting);

	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyCharacter_Countess::Interact);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter_Countess::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter_Countess::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter_Countess::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter_Countess::Pitch);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &AMyCharacter_Countess::CameraZoom);
}

void AMyCharacter_Countess::UpDown(float Value)
{
	if (Value == 0.f)
		return;

	UpDownValue = Value;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter_Countess::LeftRight(float Value)
{
	if (Value == 0.f)
		return;

	LeftRightValue = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter_Countess::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter_Countess::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter_Countess::CameraZoom(const float Value)
{
	if (Value == 0.f || !MyPlayerController)
		return;

	const float NewTargetArmLength = SpringArm->TargetArmLength + Value * 10.f;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, 500.f, 1000.f);
}

void AMyCharacter_Countess::Sprint()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
	}
}

void AMyCharacter_Countess::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

void AMyCharacter_Countess::Attack()
{
	if (IsAttacking)
		return;
	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSection(AttackIndex);
		AttackIndex = (AttackIndex + 1) % 2;
		IsAttacking = true;
	}
}

void AMyCharacter_Countess::Attack_Q()
{
	if (IsAttackingQ)
		return;
	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingQ = true;
		Remaining_SkillQ = 3;

		AttackQMove = true;
		AnimInstance->PlayAttack_Q();
		Stat->OnAttacking(Mana);

		if (MyWorld)
			MyWorld->GetTimerManager().SetTimer(QSkillHandle, this, &AMyCharacter_Countess::EndAttackingQ, 1.f, true);
	}
}

void AMyCharacter_Countess::Attack_E()
{
	if (IsAttackingE)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingE = true;
		Remaining_SkillE = 4;

		AnimInstance->PlayAttack_E();
		Stat->OnAttacking(Mana + 5);

		if (MyWorld)
			MyWorld->GetTimerManager().SetTimer(ESkillHandle, this, &AMyCharacter_Countess::EndAttackingE, 1.f, true);
	}
}

void AMyCharacter_Countess::Attack_R()
{
	if (IsAttackingR)
		return;
	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingR = true;
		Remaining_SkillR = 5;

		TArray<FHitResult> hits = AttackR_CollisionCheck();

		AnimInstance->PlayAttack_R();
		Stat->OnAttacking(Mana + 10);

		if (MyWorld)
			MyWorld->GetTimerManager().SetTimer(RSkillHandle, this, &AMyCharacter_Countess::EndAttackingR, 1.f, true);
	}
}

void AMyCharacter_Countess::EndAttackingQ()
{
	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
	AttackQMove = false;

	if (Remaining_SkillQ < 1)
	{
		const FString Q_Str = FString::Printf(TEXT("Q"));
		MyHUD->Text_Q->SetText(FText::FromString(Q_Str));
		IsAttackingQ = false;
		MyWorld->GetTimerManager().ClearTimer(QSkillHandle);
	}
	else
	{
		const FString Q_Str = FString::Printf(TEXT("%01d "), Remaining_SkillQ);
		MyHUD->Text_Q->SetText(FText::FromString(Q_Str));
	}
	--Remaining_SkillQ;
}

void AMyCharacter_Countess::EndAttackingE()
{
	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);

	if (Remaining_SkillE < 1)
	{
		const FString E_Str = FString::Printf(TEXT("E"));
		MyHUD->Text_E->SetText(FText::FromString(E_Str));
		IsAttackingE = false;
		MyWorld->GetTimerManager().ClearTimer(ESkillHandle);
	}
	else
	{
		const FString E_Str = FString::Printf(TEXT("%01d "), Remaining_SkillE);
		MyHUD->Text_E->SetText(FText::FromString(E_Str));
	}
	--Remaining_SkillE;
}

void AMyCharacter_Countess::EndAttackingR()
{
	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);

	if (Remaining_SkillR < 1)
	{
		const FString R_Str = FString::Printf(TEXT("R"));
		MyHUD->Text_R->SetText(FText::FromString(R_Str));
		IsAttackingR = false;
		MyWorld->GetTimerManager().ClearTimer(RSkillHandle);
	}
	else
	{
		const FString R_Str = FString::Printf(TEXT("%01d "), Remaining_SkillR);
		MyHUD->Text_R->SetText(FText::FromString(R_Str));
	}
	--Remaining_SkillR;
}

FHitResult AMyCharacter_Countess::AttackCollisionCheck()
{
	// 피격 판정 (충돌범위)
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 120.f;
	float AttackRadius = 60.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	/*FVector Vec = GetActorForwardVector() *AttackRange;
	FVector Center = GetActorLocation() + Vec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat Rotation = FRotationMatrix::MakeFromZ(Vec).ToQuat();
	FColor DrawColor;
	if (bResult)
		DrawColor = FColor::Green;
	else
		DrawColor = FColor::Red;

	DrawDebugCapsule(GetWorld(), Center, HalfHeight, AttackRadius,
		Rotation, DrawColor, false, 2.f);*/

	return HitResult;
}

TArray<FHitResult> AMyCharacter_Countess::AttackE_CollisionCheck()
{
	TArray<FHitResult> hits;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 120.f;
	float AttackRadius = 70;
	FVector SocketLocation = GetMesh()->GetSocketLocation("skill_socket");

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));

	bool bResult = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		SocketLocation,
		SocketLocation + GetActorForwardVector() * AttackRange,
		50.f,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		hits,
		true);

	return hits;
}

TArray<FHitResult> AMyCharacter_Countess::AttackR_CollisionCheck()
{
	TArray<FHitResult> hits;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 200.f;
	float AttackRadius = 70;

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));

	bool bResult = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		50.f,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		hits,
		true);

	return hits;
}

void AMyCharacter_Countess::AttackCheck()
{
	FHitResult HitResult = AttackCollisionCheck();

	if (HitResult.Actor.IsValid())
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}

void AMyCharacter_Countess::AttackCheck_Q()
{
	FHitResult HitResult = AttackCollisionCheck();

	if (HitResult.Actor.IsValid())
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack_Q(), DamageEvent, GetController(), this);
	}
}

void AMyCharacter_Countess::AttackCheck_E()
{
	TArray<FHitResult> hits = AttackE_CollisionCheck();

	for (auto hit : hits)
	{
		if (hit.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			hit.Actor->TakeDamage(Stat->GetAttack_E(), DamageEvent, GetController(), this);
		}
	}
}

void AMyCharacter_Countess::AttackCheck_R()
{
	TArray<FHitResult> hits = AttackR_CollisionCheck();
	for (auto hit : hits)
	{
		if (hit.Actor.IsValid())
		{
			FDamageEvent DamageEvent;
			hit.Actor->TakeDamage(Stat->GetAttack_R(), DamageEvent, GetController(), this);
		}
	}
}

void AMyCharacter_Countess::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AMyCharacter_Countess::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyCharacter_Countess::LevelUp()
{
	CountessLevel++;
	MyGameInstanceRef->SetNowLevel(CountessLevel);
	Stat->SetPlayerLevel(CountessLevel);

	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
	if (MyHUD)
	{
		MyHUD->SetCountess();
	}

	if (MyWorld)
	{
		if (LevelUpEffect)
			UGameplayStatics::SpawnEmitterAtLocation(MyWorld, LevelUpEffect, GetActorTransform());
		if (LevelUpSound)
			UGameplayStatics::SpawnSoundAtLocation(MyWorld, LevelUpSound, GetActorLocation());
	}
	MyGameInstanceRef->SetPlayFirstMapSequence(true);

}

void AMyCharacter_Countess::OnDeath_Implementation()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
	if (NowRemainingCount <= 0)
	{
		MyGameInstanceRef->SetRemainingCount(3);
		UGameplayStatics::OpenLevel(GetWorld(), FName("StartMenuMap"));
	}
	else
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Restart);
	}
}

void AMyCharacter_Countess::DoubleJump()
{
	if (DoubleJumpCounter == 0)
	{
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, 600.f), false, true);
		IsDoubleJumping = false;
		DoubleJumpCounter++;
	}

	else if (DoubleJumpCounter == 1)
	{
		ACharacter::LaunchCharacter(FVector(0.f, 0.f, 600.f), false, true);
		IsDoubleJumping = true;
	}
	else
		return;
}

void AMyCharacter_Countess::ToggleInventory()
{
	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Inventory);
	}
	else
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame);
	}
}

void AMyCharacter_Countess::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

void AMyCharacter_Countess::CheckForInteractable(APickup* PickItem)
{
	if (PickItem == NULL)
	{
		CurrentInteractable = nullptr;
		return;
	}
	CurrentInteractable = PickItem;
}

void AMyCharacter_Countess::PlayerRespawn()
{
	MyGameInstanceRef->IsFirst = true;
	int32 NowLevel = MyGameInstanceRef->GetNowLevel();
	Stat->SetPlayerLevel(NowLevel);
}

bool AMyCharacter_Countess::AddItemToInventory(APickup* Item)
{
	if (Item != NULL)
	{
		UMyInventoryHUD* InventoryHUD = Cast<UMyInventoryHUD>(MyPlayerController->InventoryWidget);

		if (Item->ItemName == "Hp" && InventoryHUD)
		{
			HpCount++;
			MyGameInstanceRef->SetHpCount(HpCount);
			CurrentInteractable = nullptr;

			return true;
		}
		else if (Item->ItemName == "Mana" && InventoryHUD)
		{
			ManaCount++;
			MyGameInstanceRef->SetManaCount(ManaCount);
			CurrentInteractable = nullptr;
			//UE_LOG(LogTemp, Warning, TEXT("Mana Count is : %d"), ManaCount);
			return true;
		}
	}
	return false;
}

void AMyCharacter_Countess::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}