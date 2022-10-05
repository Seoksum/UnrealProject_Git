// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalToElvenRuins.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameModeBase.h"
#include "MyHUD.h"

// Sets default values
APortalToElvenRuins::APortalToElvenRuins()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
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
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortalToElvenRuins::OnBeginOverlap);

	MyGameMode = Cast<AMyGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

}

// Called when the game starts or when spawned
void APortalToElvenRuins::BeginPlay()
{
	Super::BeginPlay();
	
}


void APortalToElvenRuins::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UMyHUD* MyHUD = Cast<UMyHUD>(MyGameMode->CurrentWidget);
	//MyHUD->SetMSG();

	FString GameModeURL = "?Game=/Game/Blueprints/BP_MyGameModeBase.BP_MyGameModeBase_C";
	UGameplayStatics::OpenLevel(GetWorld(), FName("ElvenRuins3"), false, GameModeURL);
}

