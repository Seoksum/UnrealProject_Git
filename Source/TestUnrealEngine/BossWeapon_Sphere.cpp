// Fill out your copyright notice in the Description page of Project Settings.


#include "BossWeapon_Sphere.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyCharacter.h"


ABossWeapon_Sphere::ABossWeapon_Sphere()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/ParagonMinions/FX/Meshes/PlayerBuffs/SM_Buff_Sphere.SM_Buff_Sphere'"));
	if (SM.Succeeded())
		MeshComp->SetStaticMesh(SM.Object);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetRelativeScale3D(FVector(1.5f,1.5f, 1.5f));
	MeshComp->SetCollisionProfileName(FName("BossWeapon"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComp->SetSphereRadius(75.f);
	//CollisionComp->BodyInstance.SetCollisionProfileName("BossWeapon");
	CollisionComp->SetCollisionProfileName(FName("BossWeapon"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(true);
	CollisionComp->SetupAttachment(MeshComp);


	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ABossWeapon_Sphere::OnBeginOverlap);

	InitialLifeSpan = 50.f;
}

void ABossWeapon_Sphere::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(20.f, DamageEvent, nullptr, this);

	/*AMyCharacter* Greystone = Cast<AMyCharacter>(OtherActor);
	if (Greystone)
	{
		Greystone->TakeDamage(20.f, DamageEvent, Greystone->GetController(), this);
	}*/
}






