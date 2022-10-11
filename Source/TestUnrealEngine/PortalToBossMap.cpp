// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalToBossMap.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


APortalToBossMap::APortalToBossMap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("ROOT"));
	RootComponent = RootScene;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PM(TEXT("StaticMesh'/Game/ParagonMinions/FX/Meshes/Targeting/SM_TargettingPortal.SM_TargettingPortal'"));
	if (PM.Succeeded())
		MeshComp->SetStaticMesh(PM.Object);
	MeshComp->SetupAttachment(RootComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));
	CollisionBox->SetRelativeLocation(FVector(2.f, 0.f, 0.f));
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &APortalToBossMap::OnBeginOverlap);

}

void APortalToBossMap::BeginPlay()
{
	Super::BeginPlay();
	
}

void APortalToBossMap::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("BossMap"));
}

