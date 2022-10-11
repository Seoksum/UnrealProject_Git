// Fill out your copyright notice in the Description page of Project Settings.


#include "Title_PC.h"
#include "StartMenuWidget.h"
#include "Kismet/GameplayStatics.h"


ATitle_PC::ATitle_PC()
{
	static ConstructorHelpers::FClassFinder<UStartMenuWidget> WB_Title(TEXT("WidgetBlueprint'/Game/UI/WBP_StartMenu.WBP_StartMenu_C'"));
	if (WB_Title.Succeeded())
	{
		TitleUIClass = WB_Title.Class;
	}
}

void ATitle_PC::BeginPlay()
{
	Super::BeginPlay();

	TitleWidget = CreateWidget<UStartMenuWidget>(this, TitleUIClass);
	TitleWidget->AddToViewport();

	SetShowMouseCursor(true);
	SetInputMode(FInputModeUIOnly());
}


void ATitle_PC::CreateServer()
{
	//UGameplayStatics::OpenLevel(GetWorld(), FName("SelectMap_2"), true, ((FString)(L"Listen")));
	UGameplayStatics::OpenLevel(GetWorld(), FName("SelectMap_2"));
}
