// Fill out your copyright notice in the Description page of Project Settings.


#include "OpeningDoor.h"
#include "FirstMapLevelScriptActor.h"


AOpeningDoor::AOpeningDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	RootComponent = Mesh;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DM(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Misc/Exo_Deco02/StaticMesh/SM_Exo_Decos_Door01.SM_Exo_Decos_Door01'"));
	if (DM.Succeeded())
		Mesh->SetStaticMesh(DM.Object);
}

// Called when the game starts or when spawned
void AOpeningDoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AOpeningDoor::RotateDoor()
{
	FRotator NowRotator = this->GetActorRotation();
	NowRotator.Yaw += YawValue;
	this->SetActorRotation(NowRotator);

}