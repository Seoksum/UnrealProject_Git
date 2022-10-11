// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartMapWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"


void URestartMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RespawnBtn->OnClicked.AddDynamic(this, &URestartMapWidget::ClickRespawnBtn);
	TitleBtn->OnClicked.AddDynamic(this, &URestartMapWidget::ClickTitleBtn);

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();

		const FString RMCount = FString::Printf(TEXT("%d"), NowRemainingCount);
		RemainingText->SetText(FText::FromString(RMCount));

		NowRemainingCount--;
		MyGameInstanceRef->SetRemainingCount(NowRemainingCount);
	}
}

void URestartMapWidget::ClickRespawnBtn()
{
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame);
		MyPlayerController->SetPause(false);

		CallPlayerRespawn();
	}

}

void URestartMapWidget::ClickTitleBtn()
{
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame);
		CallPlayerRespawn();
	}

	UGameplayStatics::OpenLevel(GetWorld(), FName("StartMenuMap"));
}

void URestartMapWidget::CallPlayerRespawn()
{
	AMyCharacter* Greystone = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Greystone)
		Greystone->PlayerRespawn();

	AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Countess)
		Countess->PlayerRespawn();

	AMyCharacter_Sparrow* Sparrow = Cast<AMyCharacter_Sparrow>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Sparrow)
		Sparrow->PlayerRespawn();
}
