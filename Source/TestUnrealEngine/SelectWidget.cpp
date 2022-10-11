// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "CharacterSelector.h"
#include "MyGameModeBase.h"
#include "CharacterSelectController.h"
#include "MyGameInstance.h"
#include "MyPlayerController.h"


// 캐릭터 선택 창에서 캐릭터 선택을 위한 widget 
void USelectWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UWorld* World = GetWorld();
	if (World)
	{
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}

	if (Btn_Greystone == nullptr || Btn_Countess == nullptr)
		return;

	Btn_Greystone->OnClicked.AddDynamic(this, &USelectWidget::GreystonePressed);
	Btn_Countess->OnClicked.AddDynamic(this, &USelectWidget::CountessPressed);
	Btn_Sparrow->OnClicked.AddDynamic(this, &USelectWidget::SparrowPressed);

	Btn_SelectCharacter->OnClicked.AddDynamic(this, &USelectWidget::SelectPressed);

	NextBtn->OnClicked.AddDynamic(this, &USelectWidget::NextButtonPressed);
	BeforeBtn->OnClicked.AddDynamic(this, &USelectWidget::BeforeButtonPressed);

}

void USelectWidget::GreystonePressed()
{
	ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
	
	SelectorActor->SelectCharacter(ECharacterIndex::Greystone);
	NowCharacterIndex = ECharacterIndex::Greystone;
}

void USelectWidget::CountessPressed()
{
	ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
	
	SelectorActor->SelectCharacter(ECharacterIndex::Countess);
	NowCharacterIndex = ECharacterIndex::Countess;
}

void USelectWidget::SparrowPressed()
{
	ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));

	SelectorActor->SelectCharacter(ECharacterIndex::Sparrow);
	NowCharacterIndex = ECharacterIndex::Sparrow;
}

void USelectWidget::SelectPressed()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ACharacterSelectController* PlayerController =
			Cast<ACharacterSelectController>(World->GetFirstPlayerController());
		
		if (PlayerController)
		{
			PlayerController->HideCharacterMenu();
		}
		MyGameInstanceRef->SetCharacterTypeIndex(NowCharacterIndex);

		FString GameModeURL = "?Game=/Game/Blueprints/BP_MyGameModeBase.BP_MyGameModeBase_C";
		UGameplayStatics::OpenLevel(GetWorld(), FName("FirstMap"), false, GameModeURL);		
			
	}
	
}

void USelectWidget::NextButtonPressed()
{
	if (NowCharacterIndex == ECharacterIndex::Greystone)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->GreystoneNext();
	}
	else if (NowCharacterIndex == ECharacterIndex::Countess)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->CountessNext();
	}
	else if (NowCharacterIndex == ECharacterIndex::Sparrow)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->SparrowNext();
	}
}

void USelectWidget::BeforeButtonPressed()
{
	if (NowCharacterIndex == ECharacterIndex::Greystone)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->GreystoneBefore();
	}
	else if (NowCharacterIndex == ECharacterIndex::Countess)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->CountessBefore();
	}
	else if (NowCharacterIndex == ECharacterIndex::Sparrow)
	{
		ACharacterSelector* SelectorActor = Cast<ACharacterSelector>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
		SelectorActor->SparrowBefore();
	}
}


