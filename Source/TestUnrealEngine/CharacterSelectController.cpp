// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelectController.h"
#include "SelectWidget.h"
#include "StartMenuWidget.h"
#include "Kismet/GameplayStatics.h"


ACharacterSelectController::ACharacterSelectController()
{

}

void ACharacterSelectController::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterSelectController::CreateCharacterMenu()
{
	if (BP_SelectWidget != nullptr)
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;
		
		SelectWidget = CreateWidget<USelectWidget>(this, BP_SelectWidget);
		SelectWidget->AddToViewport();
	}
}

void ACharacterSelectController::HideCharacterMenu()
{
	if (BP_SelectWidget != nullptr)
	{
		SelectWidget->RemoveFromParent();
		SelectWidget->Destruct();

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;
	}
}


