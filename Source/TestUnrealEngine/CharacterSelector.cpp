// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterSelector.h"
#include "CharacterSelectController.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"


// Sets default values
ACharacterSelector::ACharacterSelector()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = RootScene;


	// Greystone
	Greystones = CreateDefaultSubobject<USceneComponent>(TEXT("Greystones"));
	Greystones->SetupAttachment(RootComponent);

	Greystone1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREYSTONE1"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GM1(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone'"));
	if (GM1.Succeeded())
	{
		Greystone1->SetSkeletalMesh(GM1.Object);
		Greystone1->SetupAttachment(Greystones);
	}

	Greystone2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREYSTONE2"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GM2(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Novaborn/Meshes/Greystone_Novaborn.Greystone_Novaborn'"));
	if (GM2.Succeeded())
	{
		Greystone2->SetSkeletalMesh(GM2.Object);
		Greystone2->SetupAttachment(Greystones); 
	}

	Greystone3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREYSTONE3"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GM3(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Tough/Meshes/Greystone_Tough.Greystone_Tough'"));
	if (GM3.Succeeded())
	{
		Greystone3->SetSkeletalMesh(GM3.Object);
		Greystone3->SetupAttachment(Greystones);
	}

	Greystone4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GREYSTONE4"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> GM4(TEXT("SkeletalMesh'/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Dragonlord/Meshes/Greystone_Dragonlord.Greystone_Dragonlord'"));
	if (GM4.Succeeded())
	{
		Greystone4->SetSkeletalMesh(GM4.Object);
		Greystone4->SetupAttachment(Greystones);
	}

	//Countess
	Countesses = CreateDefaultSubobject<USceneComponent>(TEXT("Countesses"));
	Countesses->SetupAttachment(RootComponent);
	
	Countess1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("COUNTESS1"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CM1(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Meshes/SM_Countess.SM_Countess'"));
	if (CM1.Succeeded())
	{
		Countess1->SetSkeletalMesh(CM1.Object);
		Countess1->SetupAttachment(Countesses);
	}

	Countess2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("COUNTESS2"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CM2(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Skins/Tier1/Count_Carnivale/Meshes/SM_Countess_Carnivale.SM_Countess_Carnivale'"));
	if (CM2.Succeeded())
	{
		Countess2->SetSkeletalMesh(CM2.Object);
		Countess2->SetupAttachment(Countesses);
	}

	Countess3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("COUNTESS3"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CM3(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Skins/Tier2/Shogun/Meshes/SM_Countess_Shogun.SM_Countess_Shogun'"));
	if (CM3.Succeeded())
	{
		Countess3->SetSkeletalMesh(CM3.Object);
		Countess3->SetupAttachment(Countesses);
	}

	Countess4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("COUNTESS4"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CM4(TEXT("SkeletalMesh'/Game/ParagonCountess/Characters/Heroes/Countess/Skins/Tier1/Count_Gold/Meshes/SM_CountessGilded.SM_CountessGilded'"));
	if (CM4.Succeeded())
	{
		Countess4->SetSkeletalMesh(CM4.Object);
		Countess4->SetupAttachment(Countesses);
	}

	//Sparrow
	Sparrows = CreateDefaultSubobject<USceneComponent>(TEXT("Sparrows"));
	Sparrows->SetupAttachment(RootComponent);

	Sparrow1 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPARROW1"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM1(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow'"));
	if (SM1.Succeeded())
	{
		Sparrow1->SetSkeletalMesh(SM1.Object);
		Sparrow1->SetupAttachment(Sparrows);
	}

	Sparrow2 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPARROW2"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM2(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/AutumnFire/Meshes/Sparrow_AutumnFire.Sparrow_AutumnFire'"));
	if (SM2.Succeeded())
	{
		Sparrow2->SetSkeletalMesh(SM2.Object);
		Sparrow2->SetupAttachment(Sparrows);
	}

	Sparrow3 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPARROW3"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM3(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/Raven/Meshes/Sparrow_Raven.Sparrow_Raven'"));
	if (SM3.Succeeded())
	{
		Sparrow3->SetSkeletalMesh(SM3.Object);
		Sparrow3->SetupAttachment(Sparrows);
	}

	Sparrow4 = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SPARROW4"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM4(TEXT("SkeletalMesh'/Game/ParagonSparrow/Characters/Heroes/Sparrow/Skins/Rogue/Meshes/Sparrow_Rogue.Sparrow_Rogue'"));
	if (SM4.Succeeded())
	{
		Sparrow4->SetSkeletalMesh(SM4.Object);
		Sparrow4->SetupAttachment(Sparrows);
	}


}

// Called when the game starts or when spawned
void ACharacterSelector::BeginPlay()
{
	Super::BeginPlay();
	
	GreyStoneArray.SetNum(CharacterMeshCount);
	CountessArray.SetNum(CharacterMeshCount);
	SparrowArray.SetNum(CharacterMeshCount);

	GreyStoneArray[0] = Greystone1;
	GreyStoneArray[1] = Greystone2;
	GreyStoneArray[2] = Greystone3;
	GreyStoneArray[3] = Greystone4;

	CountessArray[0] = Countess1;
	CountessArray[1] = Countess2;
	CountessArray[2] = Countess3;
	CountessArray[3] = Countess4;

	SparrowArray[0] = Sparrow1;
	SparrowArray[1] = Sparrow2;
	SparrowArray[2] = Sparrow3;
	SparrowArray[3] = Sparrow4;

	HideGreystone();
	HideCountess();
	HideSparrow();
	GreyStoneArray[0]->SetVisibility(true);

	UWorld* World = GetWorld();
	if (World)
	{
		ACharacterSelectController* PlayerController =
			Cast<ACharacterSelectController>(World->GetFirstPlayerController());

		if (PlayerController)
		{
			PlayerController->CreateCharacterMenu();
		}
	}

}

// Called every frame
void ACharacterSelector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterSelector::SelectCharacter(int32 Index1)
{
	if (Index1 == ECharacterIndex::Greystone)
	{
		GreyStoneArray[0]->SetVisibility(true);
		NowGreystone = 0;
		HideCountess();
		HideSparrow();
	}
	else if(Index1== ECharacterIndex::Countess)
	{
		Countess1->SetVisibility(true);
		NowCountess = 0;
		HideGreystone(); 
		HideSparrow();
	}
	else if (Index1 == ECharacterIndex::Sparrow)
	{
		Sparrow1->SetVisibility(true);
		NowSparrow = 0;
		HideGreystone();
		HideCountess();
	}
}

void ACharacterSelector::GreystoneNext()
{
	NowGreystone = (NowGreystone + 1) % GreyStoneArray.Num();

	for (int32 i = 0; i < GreyStoneArray.Num(); i++)
	{
		if(i==NowGreystone)
			GreyStoneArray[i]->SetVisibility(true);
		else
			GreyStoneArray[i]->SetVisibility(false);
	}

	HideCountess();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowGreystone);
}

void ACharacterSelector::GreystoneBefore()
{
	NowGreystone = (NowGreystone + (GreyStoneArray.Num()-1) ) % GreyStoneArray.Num();

	for (int32 i = 0; i < GreyStoneArray.Num(); i++)
	{
		if (i == NowGreystone)
			GreyStoneArray[i]->SetVisibility(true);
		else
			GreyStoneArray[i]->SetVisibility(false);
	}

	HideCountess();
	HideSparrow();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowGreystone);
}


void ACharacterSelector::CountessNext()
{
	NowCountess = (NowCountess + 1) % CountessArray.Num();

	for (int32 i = 0; i < CountessArray.Num(); i++)
	{
		if (i == NowCountess)
			CountessArray[i]->SetVisibility(true);
		else
			CountessArray[i]->SetVisibility(false);
	}

	HideGreystone();
	HideSparrow();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowCountess);

}

void ACharacterSelector::CountessBefore()
{
	NowCountess = (NowCountess + (CountessArray.Num() - 1)) % CountessArray.Num();

	for (int32 i = 0; i < CountessArray.Num(); i++)
	{
		if (i == NowCountess)
			CountessArray[i]->SetVisibility(true);
		else
			CountessArray[i]->SetVisibility(false);
	}

	HideGreystone();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowCountess);
}

void ACharacterSelector::SparrowNext()
{
	NowSparrow = (NowSparrow + 1) % SparrowArray.Num();

	for (int32 i = 0; i < SparrowArray.Num(); i++)
	{
		if (i == NowSparrow)
			SparrowArray[i]->SetVisibility(true);
		else
			SparrowArray[i]->SetVisibility(false);
	}

	HideGreystone();
	HideCountess();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowSparrow);
}

void ACharacterSelector::SparrowBefore()
{
	NowSparrow = (NowSparrow + (SparrowArray.Num() - 1)) % SparrowArray.Num();

	for (int32 i = 0; i < SparrowArray.Num(); i++)
	{
		if (i == NowSparrow)
			SparrowArray[i]->SetVisibility(true);
		else
			SparrowArray[i]->SetVisibility(false);
	}

	HideGreystone();
	HideCountess();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetMeshIndex(NowSparrow);
}


void ACharacterSelector::HideGreystone()
{
	for (auto Greystone : GreyStoneArray)
		Greystone->SetVisibility(false);
}

void ACharacterSelector::HideCountess()
{
	for (auto Countess : CountessArray)
		Countess->SetVisibility(false);
}

void ACharacterSelector::HideSparrow()
{
	for (auto Sparrow : SparrowArray)
		Sparrow->SetVisibility(false);
}
