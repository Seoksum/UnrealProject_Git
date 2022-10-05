// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalToFirstMap.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"
#include "MyCharacter.h"
#include "MyCharacter_Countess.h"

// Sets default values
APortalToFirstMap::APortalToFirstMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = RootScene;

	PortalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PM(TEXT("StaticMesh'/Game/ParagonMinions/FX/Meshes/Targeting/SM_TargettingPortal_1.SM_TargettingPortal_1'"));
	if (PM.Succeeded())
		PortalMesh->SetStaticMesh(PM.Object);
	PortalMesh->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	CollisionBox->SetRelativeLocation(FVector(2.f, 0.f, 0.f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortalToFirstMap::OnBeginOverlap);

	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called when the game starts or when spawned
void APortalToFirstMap::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalToFirstMap::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//FString GameModeURL = "?Game=/Game/Blueprints/BP_MyGameModeBase.BP_MyGameModeBase_C";
	//UGameplayStatics::OpenLevel(GetWorld(), FName("ElvenRuins3"), false, GameModeURL);

	AMyCharacter* Greystone = Cast<AMyCharacter>(OtherActor);
	AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(OtherActor);


	//if (Greystone && Check)
	//{
	//	Greystone->SetSecondText();
	//	MyGameMode->ToFirstMap = false;
	//}
	//else if (Greystone && !Check)
	//{
	//	Greystone->SetLastText();
	//}
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("FirstMap"));
}
