// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter_Sparrow.h"
#include "SparrowAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyGameModeBase.h"
#include "MyStatComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "MyCharacterrWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Weapon_Arrow.h"
#include "ArrowWidget.h"
#include "Engine/World.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "MyHUD.h"
#include "Components/TextBlock.h"
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
#include "GameFramework/ProjectileMovementComponent.h"
#include "Square.h"
#include "DefaultMapLevelScriptActor.h"




// Sets default values
AMyCharacter_Sparrow::AMyCharacter_Sparrow()
{
	PrimaryActorTick.bCanEverTick = true;

	MyWorld = GetWorld();

	const ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/MainCharacter/ABP_SparrowAnim.ABP_SparrowAnim'"));
	GetMesh()->SetAnimInstanceClass(AnimObj.Object->GeneratedClass);

	GetCharacterMovement()->JumpZVelocity = 500.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);
	SpringArm->TargetArmLength = 500.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));

	// ZoomIn할 때 카메라 위치 
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(GetMesh());
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Stat 컴포넌트
	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));

	// HpBar 컴포넌트
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 220.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	// HpBar위젯 받아오기 
	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}

	// 현재 PlayerController
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget); // 스킬이미지 위젯
		if (MyHUD)
		{
			MyHUD->SetSparrow(); // Sparrow 스킬 이미지 설정
		}
	}

}

// Called when the game starts or when spawned
void AMyCharacter_Sparrow::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

	// DefaultMap의 레벨블루프린트로부터 델레게이트 호출받음
	DefaultScriptActor = Cast<ADefaultMapLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (DefaultScriptActor)
	{
		DefaultScriptActor->OnLevelUp.AddUObject(this, &AMyCharacter_Sparrow::LevelUp);
	}
}

void AMyCharacter_Sparrow::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MyWorld)
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	if (MyGameInstanceRef)
	{
		// GameInstance를 통해 Level을 받아와 현재 Stat컴포넌트의 Level값 설정
		MyGameInstanceRef->SetStatComp(Stat);
		SparrowLevel = MyGameInstanceRef->GetNowLevel();
		Stat->SetPlayerLevel(SparrowLevel);
	}

	AnimInstance = Cast<USparrowAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter_Sparrow::OnAttackMontageEnded);
	}

	HpBar->InitWidget();

	// HpBar 업데이트 부분(델리게이트, 바인딩)
	UMyCharacterrWidget* HpWidget = Cast<UMyCharacterrWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(Stat);
		HpWidget->BindMana(Stat);
	}

	// Arrow 속도 업데이트(Percent로 표시)
	if (BP_ArrowWidget)
	{
		MyArrowWidget = CreateWidget<UArrowWidget>(MyWorld, BP_ArrowWidget);
		MyArrowWidget->AddToViewport();
		MyArrowWidget->BindArrowSpeed(this);
	}
}

void AMyCharacter_Sparrow::Landed(const FHitResult& Hit)
{
	IsFallingNow = false;
	DoubleJumpCounter = 0;
}

// Called every frame
void AMyCharacter_Sparrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 벽 위에 있으며 올라가는 중이라면
	if (IsClimbingUp && bIsOnWall)
	{
		FVector Loc = GetActorLocation();
		SetActorLocation(FVector(Loc.X, Loc.Y, Loc.Z + 1.5f));
	}

	// 벽 위를 더 이상 오를 수 없어 착지해야할 때
	if (IsClimbingComplete)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * 4.f);
		SetActorLocation(GetActorLocation() + GetActorUpVector() * 7.f);
	}

	// 오른쪽 마우스키를 누르고 있을 때(화살 속도값 증가)
	if (IsRightMouse)
	{
		if (!SkillAttackEnded)
			ArrowSpeedVal += 10.f;

		if (ArrowSpeedVal >= 2200.f)
			ArrowSpeedVal = 2200.f;

		OnArrowSpeedChanged.Broadcast();
	}

	// 오른쪽 마우스키를 떼고 있을 때(화살 속도값 감소)
	else
	{
		ArrowSpeedVal -= 10.f;
		if (ArrowSpeedVal <= 400.f)
			ArrowSpeedVal = 400.f;
		OnArrowSpeedChanged.Broadcast();
	}
}

// Called to bind functionality to input
void AMyCharacter_Sparrow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::DoubleJump);

	PlayerInputComponent->BindAction(TEXT("ArrowZoomIn"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::ArrowZoomIn);
	PlayerInputComponent->BindAction(TEXT("ArrowZoomIn"), EInputEvent::IE_Released, this, &AMyCharacter_Sparrow::ArrowZoomOut);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Released, this, &AMyCharacter_Sparrow::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack_Q"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::Attack_Q);
	PlayerInputComponent->BindAction(TEXT("Attack_E"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::Attack_E);
	PlayerInputComponent->BindAction(TEXT("Attack_R"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::Attack_R);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyCharacter_Sparrow::StopSprinting);

	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::Interact);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter_Sparrow::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter_Sparrow::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter_Sparrow::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter_Sparrow::Pitch);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &AMyCharacter_Sparrow::CameraZoom);

	PlayerInputComponent->BindAction(TEXT("ClimbingUp"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::PressClimbingUp);
	PlayerInputComponent->BindAction(TEXT("ClimbingDown"), EInputEvent::IE_Pressed, this, &AMyCharacter_Sparrow::PressClimbingComplete);

}

void AMyCharacter_Sparrow::UpDown(float Value)
{
	if (Value == 0.f || bIsOnWall)
		return;

	if (IsClimbingComplete)
		AddMovementInput(GetActorForwardVector(), Value);

	UpDownValue = Value;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter_Sparrow::LeftRight(float Value)
{
	if (Value == 0.f || bIsOnWall)
		return;

	LeftRightValue = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter_Sparrow::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter_Sparrow::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter_Sparrow::CameraZoom(const float Value)
{
	if (Value == 0.f || !MyPlayerController)
		return;

	const float NewTargetArmLength = SpringArm->TargetArmLength + Value * 10.f;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, 500.f, 1000.f);
}

// Shift키를 눌러 캐릭터 속도 증가
void AMyCharacter_Sparrow::Sprint()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}
}

void AMyCharacter_Sparrow::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;
	}
}

// 오른쪽 마우스 클릭한 상태에서 마우스 휠 축을 이용해 FP_MuzzleLocation 위치로 ZoomIn해줌
void AMyCharacter_Sparrow::ArrowZoomIn()
{
	IsRightMouse = true;

	FVector EyeLocation = FP_MuzzleLocation->GetComponentLocation();
	Camera->SetRelativeLocation(EyeLocation);
	FRotator cam = FRotator(30.f, 0.f, 0.f);
	Camera->SetRelativeRotation(cam);

}

// 오른쪽 마우스를 떼 카메라를 원 위치로 이동
void AMyCharacter_Sparrow::ArrowZoomOut()
{
	IsRightMouse = false;
	ArrowSpeedVal = 400.f;
	SkillAttackEnded = false;

	SpringArm->SetRelativeLocation(FVector(-0.f, 0.f, 0.f));
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));

	Camera->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	Camera->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
}

// 왼쪽 마우스 클릭, 기본공격 
void AMyCharacter_Sparrow::Attack()
{
	if (IsAttacking)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		AnimInstance->PlayAttackMontage();
		ThrowArrow(); // 화살 속도 조절 불가능
		IsAttacking = true;
	}
}

// Q스킬
void AMyCharacter_Sparrow::Attack_Q()
{
	if (IsAttackingQ)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingQ = true;
		Remaining_SkillQ = 3; // 쿨타임 3초

		AnimInstance->PlayAttack_Q();
		ThrowArrowSpeed(); // 화살 속도 조절 가능
		Stat->OnAttacking(Mana);

		MyWorld->GetTimerManager().SetTimer(QSkillHandle, this, &AMyCharacter_Sparrow::EndAttackingQ, 1.f, true);
	}
}
// E스킬 
void AMyCharacter_Sparrow::Attack_E()
{
	if (IsAttackingE)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingE = true;
		Remaining_SkillE = 4;

		AnimInstance->PlayAttack_E();
		ThrowArrowSpeed();
		Stat->OnAttacking(Mana + 5);
		SkillAttackEnded = true;
		MyWorld->GetTimerManager().SetTimer(ESkillHandle, this, &AMyCharacter_Sparrow::EndAttackingE, 1.f, true);
	}
}
// R 스킬
void AMyCharacter_Sparrow::Attack_R()
{
	if (IsAttackingR)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingR = true;
		Remaining_SkillR = 5;
		AnimInstance->PlayAttack_R();
		ThrowArrow();

		Stat->OnAttacking(Mana + 10);

		MyWorld->GetTimerManager().SetTimer(RSkillHandle, this, &AMyCharacter_Sparrow::EndAttackingR, 1.f, true);
	}
}

// Q 스킬이 끝나면
void AMyCharacter_Sparrow::EndAttackingQ()
{
	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
	//ArrowSpeedVal = 400.f;

	if (Remaining_SkillQ < 1) // 쿨타임이 끝나면 다시 Q스킬 사용 가능
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

void AMyCharacter_Sparrow::EndAttackingE()
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

void AMyCharacter_Sparrow::EndAttackingR()
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


void AMyCharacter_Sparrow::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AMyCharacter_Sparrow::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

void AMyCharacter_Sparrow::LevelUp()
{
	SparrowLevel++;
	MyGameInstanceRef->SetNowLevel(SparrowLevel);
	Stat->SetPlayerLevel(SparrowLevel);

	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
	if (MyHUD)
	{
		MyHUD->SetSparrow();
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

void AMyCharacter_Sparrow::ToggleInventory()
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

void AMyCharacter_Sparrow::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

void AMyCharacter_Sparrow::CheckForInteractable(APickup* PickItem)
{
	if (PickItem == NULL)
	{
		CurrentInteractable = nullptr;
		return;
	}

	CurrentInteractable = PickItem;
}

bool AMyCharacter_Sparrow::AddItemToInventory(APickup* Item)
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

void AMyCharacter_Sparrow::DoubleJump()
{
	if (bIsOnWall)
		bIsOnWall = false;

	IsFallingNow = true;
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
		//DoubleJumpCounter++;
	}
	else
		return;
}

void AMyCharacter_Sparrow::OnDeath_Implementation()
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



void AMyCharacter_Sparrow::ThrowArrow()
{
	if (ArrowClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;

			FVector SpawnLocation = this->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
			SpawnLocation.X -= 10.f;
			FRotator Rotation = GetActorForwardVector().Rotation();
			Rotation.Pitch += 5.f;

			AWeapon_Arrow* Projectile = World->SpawnActor<AWeapon_Arrow>(ArrowClass, SpawnLocation, Rotation, SpawnParams);
		}
	}
}

void AMyCharacter_Sparrow::ThrowArrowSpeed()
{
	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		FVector SpawnLocation = this->GetMesh()->GetSocketLocation(FName("ProjectileSocket"));
		SpawnLocation.X -= 10.f;
		FRotator Rotation = GetActorForwardVector().Rotation();

		FTransform SpawnTransform(Rotation, SpawnLocation);


		AWeapon_Arrow* Projectile = World->SpawnActorDeferred<AWeapon_Arrow>(ArrowClass, SpawnTransform);
		if (Projectile)
		{
			Projectile->ProjectileMovement->InitialSpeed = ArrowSpeedVal;
			UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
		}
	}
}

void AMyCharacter_Sparrow::PressClimbingUp()
{
	if (IsClimbingUp)
		return;

	FVector Start = GetCapsuleComponent()->GetRelativeLocation();
	FVector End = GetCapsuleComponent()->GetRelativeLocation() + GetCapsuleComponent()->GetForwardVector() * 50.f;

	float CapsuleHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	Start.Z += CapsuleHeight * 2 + 60.f;
	End.Z += CapsuleHeight * 2 + 60.f;

	FHitResult OutHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bResult = GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);
	Wall = Cast<ASquare>(OutHit.Actor);

	FColor DrawColor;

	if (bResult && Wall)
	{
		DrawColor = FColor::Green;
		bIsOnWall = true;
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

		IsClimbingUp = true;
		MyWorld->GetTimerManager().SetTimer(ClimbingHandle, this, &AMyCharacter_Sparrow::ReleaseClimbingUp, 1.2f, true);
	}
	else
		DrawColor = FColor::Red;

	DrawDebugLine(GetWorld(), Start, End, DrawColor, false, 20.f, 5.f, 20.f);
}

void AMyCharacter_Sparrow::ReleaseClimbingUp()
{
	IsClimbingUp = false;
	MyWorld->GetTimerManager().ClearTimer(ClimbingHandle);
}

void AMyCharacter_Sparrow::PressClimbingComplete()
{
	if (bIsOnWall)
		return;
	bIsOnWall = false;
	IsClimbingComplete = true;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	MyWorld->GetTimerManager().SetTimer(ClimbingDownHandle, this, &AMyCharacter_Sparrow::ReleaseClimbingComplete, 1.f, true);
	AnimInstance->PlayClimbingComplete();
}

void AMyCharacter_Sparrow::ReleaseClimbingComplete()
{
	IsClimbingComplete = false;
	MyWorld->GetTimerManager().ClearTimer(ClimbingDownHandle);
}

void AMyCharacter_Sparrow::PlayerRespawn()
{
	MyGameInstanceRef->IsFirst = true;
	int32 NowLevel = MyGameInstanceRef->GetNowLevel();
	Stat->SetPlayerLevel(NowLevel);
}
