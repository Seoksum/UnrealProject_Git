// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalToSecondMap.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"
#include "Components/TextBlock.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"
#include "MyPlayerController.h"
#include "CharacterSelectController.h"




// Sets default values
APortalToSecondMap::APortalToSecondMap()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = RootScene;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PM(TEXT("StaticMesh'/Game/ParagonMinions/FX/Meshes/Targeting/SM_TargettingPortal.SM_TargettingPortal'"));
	if (PM.Succeeded())
		PortalMesh->SetStaticMesh(PM.Object);
	PortalMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	CollisionBox->SetRelativeLocation(FVector(2.f, 0.f, 0.f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortalToSecondMap::OnBeginOverlap);

	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called when the game starts or when spawned
void APortalToSecondMap::BeginPlay()
{
	Super::BeginPlay();

}

void APortalToSecondMap::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	//ACharacterSelectController* PC = Cast<ACharacterSelectController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	//if (PC)
	//	PC->HideFirstMapWidget();

	FString GameModeURL = "?Game=/Game/Blueprints/BP_MyGameModeBase.BP_MyGameModeBase_C";
	UGameplayStatics::OpenLevel(GetWorld(), FName("DefaultMap1"), false, GameModeURL);


}




