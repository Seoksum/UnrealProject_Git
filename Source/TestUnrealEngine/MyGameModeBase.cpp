// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyHUD.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"
#include "Components/TextBlock.h"
#include "PortalToSecondMap.h"
#include "Blueprint/UserWidget.h"
#include "MyPlayerController.h"


AMyGameModeBase::AMyGameModeBase()
{

	PlayerControllerClass = AMyPlayerController::StaticClass();

	UWorld* World = GetWorld();
	if (World)
	{
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(World));
		if (MyGameInstanceRef)
		{
			NowIndex = MyGameInstanceRef->GetCharacterTypeIndex();
			CharMeshIndex = MyGameInstanceRef->GetMeshIndex();
		}
	}

	if (NowIndex == ECharacterIndex::Greystone)
	{
		static ConstructorHelpers::FClassFinder<AMyCharacter> BP_Greystone1(TEXT("Blueprint'/Game/MainCharacter/BP_MyCharacter.BP_MyCharacter_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter> BP_Greystone2(TEXT("Blueprint'/Game/MainCharacter/BP_Greystones/BP_Greystone2.BP_Greystone2_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter> BP_Greystone3(TEXT("Blueprint'/Game/MainCharacter/BP_Greystones/BP_Greystone3.BP_Greystone3_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter> BP_Greystone4(TEXT("Blueprint'/Game/MainCharacter/BP_Greystones/BP_Greystone4.BP_Greystone4_C'"));

		if (BP_Greystone1.Succeeded() && CharMeshIndex==0)
			DefaultPawnClass = BP_Greystone1.Class;

		else if (BP_Greystone2.Succeeded() && CharMeshIndex == 1)
			DefaultPawnClass = BP_Greystone2.Class;

		else if (BP_Greystone3.Succeeded() && CharMeshIndex == 2)
			DefaultPawnClass = BP_Greystone3.Class;

		else if (BP_Greystone4.Succeeded() && CharMeshIndex == 3)
			DefaultPawnClass = BP_Greystone4.Class;
	}

	else if (NowIndex == ECharacterIndex::Countess)
	{
		static ConstructorHelpers::FClassFinder<AMyCharacter_Countess> BP_Countess1(TEXT("Blueprint'/Game/MainCharacter/BP_MyCountess.BP_MyCountess_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Countess> BP_Countess2(TEXT("Blueprint'/Game/MainCharacter/BP_Countesses/BP_Countess2.BP_Countess2_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Countess> BP_Countess3(TEXT("Blueprint'/Game/MainCharacter/BP_Countesses/BP_Countess3.BP_Countess3_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Countess> BP_Countess4(TEXT("Blueprint'/Game/MainCharacter/BP_Countesses/BP_Countess4.BP_Countess4_C'"));

		if (BP_Countess1.Succeeded() && CharMeshIndex == 0)
			DefaultPawnClass = BP_Countess1.Class;

		else if (BP_Countess2.Succeeded() && CharMeshIndex == 1)
			DefaultPawnClass = BP_Countess2.Class;

		else if (BP_Countess3.Succeeded() && CharMeshIndex == 2)
			DefaultPawnClass = BP_Countess3.Class;

		else if (BP_Countess4.Succeeded() && CharMeshIndex == 3)
			DefaultPawnClass = BP_Countess4.Class;
	}

	else if (NowIndex == ECharacterIndex::Sparrow)
	{
		static ConstructorHelpers::FClassFinder<AMyCharacter_Sparrow> BP_Sparrow1(TEXT("Blueprint'/Game/MainCharacter/BP_MySparrow.BP_MySparrow_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Sparrow> BP_Sparrow2(TEXT("Blueprint'/Game/MainCharacter/BP_Sparrows/BP_MySparrow2.BP_MySparrow2_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Sparrow> BP_Sparrow3(TEXT("Blueprint'/Game/MainCharacter/BP_Sparrows/BP_MySparrow3.BP_MySparrow3_C'"));
		static ConstructorHelpers::FClassFinder<AMyCharacter_Sparrow> BP_Sparrow4(TEXT("Blueprint'/Game/MainCharacter/BP_Sparrows/BP_MySparrow4.BP_MySparrow4_C'"));

		if (BP_Sparrow1.Succeeded() && CharMeshIndex == 0)
			DefaultPawnClass = BP_Sparrow1.Class;

		else if (BP_Sparrow2.Succeeded() && CharMeshIndex == 1)
			DefaultPawnClass = BP_Sparrow2.Class;

		else if (BP_Sparrow3.Succeeded() && CharMeshIndex == 2)
			DefaultPawnClass = BP_Sparrow3.Class;

		else if (BP_Sparrow4.Succeeded() && CharMeshIndex == 3)
			DefaultPawnClass = BP_Sparrow4.Class;
	}

	PlayerControllerClass = AMyPlayerController::StaticClass();

}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
}

void AMyGameModeBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SetNowIndex(NowIndex);
	

}

void AMyGameModeBase::SetNowIndex(int32 Index)
{
	NowIndex = Index;
}





