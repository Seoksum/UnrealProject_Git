// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyHUD.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyHUD.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "MyInventoryHUD.h"
#include "MyGameInstance.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"



AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder<UMyHUD> Ingame_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_HUD.WBP_HUD_C'"));
	if (Ingame_Class.Succeeded())
	{
		IngameHUDClass = Ingame_Class.Class;
		CurrentWidget = CreateWidget(GetWorld(), IngameHUDClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport(); // AddToViewport의 인자를 이용해서 ZOrder 순서를 정할 수 있음
			//CurrentWidget->RemoveFromViewport(); 지울 수도 있다. 
		}
	}

	static ConstructorHelpers::FClassFinder<UMyInventoryHUD> Inventory_Class(TEXT("WidgetBlueprint'/Game/UI/HUD_Inventory.HUD_Inventory_C'"));
	if (Inventory_Class.Succeeded())
	{
		InventoryHUDClass = Inventory_Class.Class;
		InventoryWidget = CreateWidget(GetWorld(), InventoryHUDClass);
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> Restart_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Restart.WBP_Restart_C'"));
	if (Restart_Class.Succeeded())
		RestartHUDClass = Restart_Class.Class;

	
	HUDState = EHUDState::HS_Ingame;
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMyPlayerController::ApplyHUDChanges()
{
	// 이전의 hud를 지우고, 새로운걸 적용
	if (CurrentWidget != nullptr)
	{
		CurrentWidget->RemoveFromParent();
	}

	switch (HUDState)
	{
	case EHUDState::HS_Ingame:
	{
		ApplyHUD(IngameHUDClass, false, false);
		
		break;
	}
	case EHUDState::HS_Inventory:
	{
		ApplyHUD(InventoryHUDClass, true, true);
		break;
	}
	case EHUDState::HS_Restart:
	{
		ApplyHUD(RestartHUDClass, true, true);
		break;
	}
	default:
	{
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
	}
}

uint8 AMyPlayerController::GetHUDState()
{
	return HUDState;
}

void AMyPlayerController::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool AMyPlayerController::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool IsShowMouseCursor, bool IsEnableClickEvents)
{
	// 적용하기전 위젯 체크
	if (WidgetToApply)
	{
		bShowMouseCursor = IsShowMouseCursor;
		bEnableClickEvents = IsEnableClickEvents;

		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), WidgetToApply);

		if (WidgetToApply == IngameHUDClass)
		{
			UMyHUD* TestHUD = Cast<UMyHUD>(CurrentWidget);
			MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			if (MyGameInstanceRef->GetCharacterTypeIndex() == ECharacterIndex::Greystone)
				TestHUD->SetGreystone();
			else if (MyGameInstanceRef->GetCharacterTypeIndex() == ECharacterIndex::Countess)
				TestHUD->SetCountess();
			else if (MyGameInstanceRef->GetCharacterTypeIndex() == ECharacterIndex::Sparrow)
				TestHUD->SetSparrow();
			
		}
		else if (WidgetToApply== InventoryHUDClass)
		{
			UMyInventoryHUD* TestHUD = Cast<UMyInventoryHUD>(CurrentWidget);
			MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

			TestHUD->SetSlot1(MyGameInstanceRef->GetHpCount());
			TestHUD->SetSlot2(MyGameInstanceRef->GetManaCount());
		}

		else if (WidgetToApply == RestartHUDClass)
		{
			SetPause(true);
		}
		

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		return false;
	}
	return false;
}




