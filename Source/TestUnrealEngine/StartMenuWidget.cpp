// Fill out your copyright notice in the Description page of Project Settings.


#include "StartMenuWidget.h"
#include "Components/Button.h"
#include "Title_PC.h"
#include "Kismet/GameplayStatics.h"


void UStartMenuWidget::NativeConstruct()
{
	StartButton->OnClicked.AddDynamic(this, &UStartMenuWidget::CreateServerHandler);

}


void UStartMenuWidget::CreateServerHandler()
{
	ATitle_PC* MyPC = Cast<ATitle_PC>(UGameplayStatics::GetPlayerController(GetWorld(),0));
	if (MyPC)
	{
		MyPC->CreateServer();
	}
}


