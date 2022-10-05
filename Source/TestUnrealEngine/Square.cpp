// Fill out your copyright notice in the Description page of Project Settings.


#include "Square.h"
#include "Components/BoxComponent.h"
#include "MyCharacter_Sparrow.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ASquare::ASquare()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	RootComponent = SceneComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	MeshComp->SetupAttachment(SceneComp);
	//CollisionComp->SetCollisionProfileName(FName("NoCollision"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> DM(TEXT("StaticMesh'/Game/InfinityBladeGrassLands/Environments/Plains/Env_Plains_Ruins/StaticMesh/SM_Plains_Wall_Straight_01.SM_Plains_Wall_Straight_01'"));
	if (DM.Succeeded())
		MeshComp->SetStaticMesh(DM.Object);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetupAttachment(SceneComp);
	CollisionComp->SetBoxExtent(FVector(10.f, 100.f, 100.f));
	CollisionComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetNotifyRigidBodyCollision(true);
	//CollisionComp->OnComponentHit.AddDynamic(this, &ASquare::OnHit);

}

// Called when the game starts or when spawned
void ASquare::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASquare::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASquare::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{ 
	AMyCharacter_Sparrow* MyCharacter = Cast<AMyCharacter_Sparrow>(OtherActor);
	if (MyCharacter)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MyCharacter is not "));
	}
	
}