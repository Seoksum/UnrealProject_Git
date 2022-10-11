// Fill out your copyright notice in the Description page of Project Settings.


#include "Fountain.h"
#include "SelectMapLevelScriptActor.h"


// Sets default values
AFountain::AFountain()
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
void AFountain::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFountain::RotateDoor()
{
	UE_LOG(LogTemp, Warning, TEXT("Open Door !"));
	FRotator NowRotator = this->GetActorRotation();
	NowRotator.Yaw += YawValue;
	this->SetActorRotation(NowRotator);

}

