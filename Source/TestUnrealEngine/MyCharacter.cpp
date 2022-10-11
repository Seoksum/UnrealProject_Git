// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
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
#include "Engine/World.h"
#include "Enemy_Dusk.h"
#include "Enemy_Down.h"
#include "MyHUD.h"
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
#include "DefaultMapLevelScriptActor.h"


// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCharacterMovement()->JumpZVelocity = 500.f;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 600.f;
	SpringArm->SetRelativeRotation(FRotator(-35.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;
	Camera->bUsePawnControlRotation = false;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("MyCharacter"));
	//GetCapsuleComponent()->SetCollisionProfileName(FName("MyCharacter"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

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

	//MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (MyPlayerController)
	{
		UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->SetGreystone();
		}
	}


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	Inventory.SetNum(4);
	CurrentInteractable = nullptr;

	DefaultScriptActor = Cast<ADefaultMapLevelScriptActor>(GetWorld()->GetLevelScriptActor());
	if (DefaultScriptActor)
	{
		DefaultScriptActor->OnLevelUp.AddUObject(this, &AMyCharacter::LevelUp);

	}

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (MyWorld)
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetStatComp(Stat);
		GreystoneLevel = MyGameInstanceRef->GetNowLevel();
		Stat->SetPlayerLevel(GreystoneLevel);
	}




	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
		AnimInstance->OnAttackHit.AddUObject(this, &AMyCharacter::AttackCheck);
		AnimInstance->OnAttackHit_Q.AddUObject(this, &AMyCharacter::AttackCheck_Q);
		AnimInstance->OnAttackHit_E.AddUObject(this, &AMyCharacter::AttackCheck_E);
		AnimInstance->OnAttackHit_R.AddUObject(this, &AMyCharacter::AttackCheck_R);
	}

	HpBar->InitWidget();

	// TODO , HpBar 업데이트 부분(델리게이트, 바인딩)
	UMyCharacterrWidget* HpWidget = Cast<UMyCharacterrWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(Stat);
		HpWidget->BindMana(Stat);
	}

}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (AttackQMove)
	{
		FVector NewLocation1 = GetActorLocation() + GetActorForwardVector() * 7.f;
		SetActorLocation(NewLocation1);
	}
}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::DoubleJump);
	// ACharacter에 Jump() 기능이 이미 존재하고 있어서 따로 만들어주지 않아도 됨

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
	PlayerInputComponent->BindAction(TEXT("Attack_Q"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack_Q);
	PlayerInputComponent->BindAction(TEXT("Attack_E"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack_E);
	PlayerInputComponent->BindAction(TEXT("Attack_R"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack_R);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AMyCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AMyCharacter::StopSprinting);

	PlayerInputComponent->BindAction(TEXT("ToggleInventory"), EInputEvent::IE_Pressed, this, &AMyCharacter::ToggleInventory);
	PlayerInputComponent->BindAction(TEXT("Interact"), EInputEvent::IE_Pressed, this, &AMyCharacter::Interact);

	PlayerInputComponent->BindAxis(TEXT("UpDown"), this, &AMyCharacter::UpDown);
	PlayerInputComponent->BindAxis(TEXT("LeftRight"), this, &AMyCharacter::LeftRight);
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AMyCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyCharacter::Pitch);
	PlayerInputComponent->BindAxis(TEXT("CameraZoom"), this, &AMyCharacter::CameraZoom);

}

void AMyCharacter::UpDown(float Value)
{
	if (Value == 0.f)
		return;

	UpDownValue = 0.5f;
	AddMovementInput(GetActorForwardVector(), Value);
}

void AMyCharacter::LeftRight(float Value)
{
	if (Value == 0.f)
		return;

	LeftRightValue = Value;
	AddMovementInput(GetActorRightVector(), Value);
}

void AMyCharacter::Yaw(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::Pitch(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter::CameraZoom(const float Value)
{
	if (Value == 0.f || !MyPlayerController)
		return;

	const float NewTargetArmLength = SpringArm->TargetArmLength + Value * 10.f;
	SpringArm->TargetArmLength = FMath::Clamp(NewTargetArmLength, 500.f, 1000.f);

}

// Shift키를 눌러 캐릭터의 이동속도를 증가시키는 함수
void AMyCharacter::Sprint()
{
	if (!bIsSprinting)
	{
		bIsSprinting = true;
		AnimInstance->Vertical = 1.f;
		GetCharacterMovement()->MaxWalkSpeed = 1200.f;
	}
}
// Shift키를 떼 캐릭터의 이동속도를 처음 값으로 설정하는 함수
void AMyCharacter::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		AnimInstance->Vertical = 0.5f;
		GetCharacterMovement()->MaxWalkSpeed = 600.f;

	}
}

// 공격 하는 함수 (기본 공격)
void AMyCharacter::Attack()
{
	if (IsAttacking)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSection(AttackIndex);

		UseAttackEffect();

		AttackIndex = (AttackIndex + 1) % 3;

		IsAttacking = true;
	}
}
// Q스킬
void AMyCharacter::Attack_Q()
{
	if (IsAttackingQ)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingQ = true;
		AttackQMove = true;
		Remaining_SkillQ = 3;

		AnimInstance->PlayAttack_Q();
		Stat->OnAttacking(Mana);

		MyWorld->GetTimerManager().SetTimer(QSkillHandle, this, &AMyCharacter::EndAttackingQ, 1.f, true);
	}
}
// E스킬
void AMyCharacter::Attack_E()
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
			MyWorld->GetTimerManager().SetTimer(ESkillHandle, this, &AMyCharacter::EndAttackingE, 1.f, true);
	}
}
// R스킬
void AMyCharacter::Attack_R()
{
	if (IsAttackingR)
		return;

	if (MyPlayerController->GetHUDState() == MyPlayerController->HS_Ingame)
	{
		IsAttackingR = true;
		Remaining_SkillR = 5;

		TArray<FHitResult> hits = AttackR_CollisionCheck();
		for (auto hit : hits)
		{
			if (hit.Actor.IsValid())
			{
				if (AttackR_Effect)
				{
					FVector SocketLocation = GetMesh()->GetSocketLocation("skill_socket");
					FTransform EffectTrasform(FRotator::ZeroRotator, SocketLocation);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), AttackEffect, EffectTrasform);
				}
			}
		}

		AnimInstance->PlayAttack_R();
		Stat->OnAttacking(Mana + 10);

		if (MyWorld)
			MyWorld->GetTimerManager().SetTimer(RSkillHandle, this, &AMyCharacter::EndAttackingR, 1.f, true);
	}
}

// 공격이 끝난 후 쿨타임 보여주는 함수
void AMyCharacter::EndAttackingQ()
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

void AMyCharacter::EndAttackingE()
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

void AMyCharacter::EndAttackingR()
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

// 공격했을 때 Sweep Single 함수를 이용해 Attack 채널과 충돌 판정 확인 
FHitResult AMyCharacter::AttackCollisionCheckBySweep()
{
	// 피격 판정 (충돌범위)
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 150.f;
	float AttackRadius = 100.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	return HitResult;

}

// 공격했을 때 SphereTraceMultiForObjects 함수를 이용해 Enemy 오브젝트 채널과 충돌 판정 확인
TArray<FHitResult> AMyCharacter::AttackCheckBySphereTrace(float AttackRange, float AttackRadius, FVector Loc)
{
	TArray<FHitResult> hits;
	FCollisionQueryParams Params(NAME_None, false, this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(UEngineTypes::ConvertToObjectType(ECC_GameTraceChannel4));

	bool bResult = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		Loc,
		Loc + GetActorForwardVector() * AttackRange,
		50.f,
		ObjectTypes,
		false,
		TArray<AActor*>(),
		EDrawDebugTrace::ForOneFrame,
		hits,
		true);
	return hits;
}

TArray<FHitResult> AMyCharacter::AttackE_CollisionCheck()
{
	FVector SocketLocation = GetMesh()->GetSocketLocation("skill_socket");
	TArray<FHitResult> hits = AttackCheckBySphereTrace(150.f,100.f, SocketLocation);

	return hits;
}

TArray<FHitResult> AMyCharacter::AttackR_CollisionCheck()
{
	TArray<FHitResult> hits = AttackCheckBySphereTrace(150.f, 100.f, GetActorLocation());
	FCollisionQueryParams Params(NAME_None, false, this);

	return hits;
}

// 충돌 결과를 이용해 적이 TakeDamage하도록 해주는 함수
void AMyCharacter::AttackCheck()
{
	FHitResult HitResult = AttackCollisionCheckBySweep();

	if (HitResult.Actor.IsValid())
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack(), DamageEvent, GetController(), this);
	}
}
void AMyCharacter::AttackCheck_Q()
{
	FHitResult HitResult = AttackCollisionCheckBySweep();

	if (HitResult.Actor.IsValid())
	{
		FDamageEvent DamageEvent;
		HitResult.Actor->TakeDamage(Stat->GetAttack_Q(), DamageEvent, GetController(), this);
	}
}

void AMyCharacter::AttackCheck_E()
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

void AMyCharacter::AttackCheck_R()
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

// Montage가 끝나면 호출되는 함수
void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

// DamageAmount만큼 캐릭터의 체력이 깎이는 함수
float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Stat->OnAttacked(DamageAmount);
	return DamageAmount;
}

// 캐릭터 레벨 업 함수
void AMyCharacter::LevelUp()
{
	GreystoneLevel++;
	MyGameInstanceRef->SetNowLevel(GreystoneLevel);
	Stat->SetPlayerLevel(GreystoneLevel);

	UMyHUD* MyHUD = Cast<UMyHUD>(MyPlayerController->CurrentWidget);
	if (MyHUD)
	{
		MyHUD->SetGreystone();
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

// 파티클시스템 발생
void AMyCharacter::UseAttackEffect()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (AttackEffect)
		{
			FVector SocketLocation = GetMesh()->GetSocketLocation("FX_Sword_Top");
			FTransform EffectTrasform(FRotator::ZeroRotator, SocketLocation);			
			UGameplayStatics::SpawnEmitterAtLocation(World, AttackEffect, EffectTrasform);
		}
	}
}

// 캐릭터의 체력이 0이 되었을 때 호출되는 함수
void AMyCharacter::OnDeath_Implementation()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
	if (NowRemainingCount <= 0) // 더 이상 남은 목숨이 없다면 처음부터 시작
	{
		MyGameInstanceRef->SetRemainingCount(3);
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame);
		UGameplayStatics::OpenLevel(GetWorld(), FName("StartMenuMap"));
	}
	else // 남은 목숨이 있다면 다른 HUD 보여주기
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Restart);
	}
}

// 스페이스를 두 번 누르면 더블 점프 가능
void AMyCharacter::DoubleJump() 
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

// 인벤토리 영역

// I 키를 눌러 인벤토리 창<->인게임 창 서로 교체 가능
void AMyCharacter::ToggleInventory()
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

// Z키를 눌러 아이템 수집
void AMyCharacter::Interact()
{
	if (CurrentInteractable != nullptr)
	{
		CurrentInteractable->Interact_Implementation();
	}
}

// 인자로 넘겨 받은 아이템을 주울 수 있는지 확인하는 함수
void AMyCharacter::CheckForInteractable(APickup* PickItem)
{

	if (PickItem == NULL)
	{
		CurrentInteractable = nullptr;
		return;
	}
	CurrentInteractable = PickItem;

}

// 인벤토리에 아이템을 추가해 수집한 아이템 개수만큼 화면에 보여주는 함수
bool AMyCharacter::AddItemToInventory(APickup* Item)
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
			return true;
		}
	}
	return false;
}

// 플레이어가 Respawn시 체력을 다시 Max값으로 설정해주는 함수 
void AMyCharacter::PlayerRespawn()
{
	MyGameInstanceRef->IsFirst = true;
	int32 NowLevel = MyGameInstanceRef->GetNowLevel();
	Stat->SetPlayerLevel(NowLevel);
}

// 캐릭터가 땅에 닿으면 점프가 다시 가능하도록 DoubleJumpCounter을 0으로 설정해주는 함수
void AMyCharacter::Landed(const FHitResult& Hit)
{
	DoubleJumpCounter = 0;
}



