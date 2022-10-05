// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"
#include "MyCharacter_Countess.h"
#include "MyCharacter_Sparrow.h"


APickup::APickup()
{
	// pickup의 mesh,itemname,helptext, itemValue 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	MeshComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	ItemName = FString("Enter an item name here...");
	InteractableHelpText = FString("Press E to pick item up");
	Value = 0;


	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(200.f);
	CollisionComp->SetupAttachment(MeshComp);
	CollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	
	//CollisionComp->BodyInstance.SetCollisionProfileName("OverlapAllDynamic");
}

void APickup::BeginPlay()
{
	InteractableHelpText = FString::Printf(TEXT("%s: Press E to pick up"), *ItemName);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &APickup::OnBeginOverlap);
}

void APickup::Interact_Implementation()
{
	AMyCharacter* Greystone = Cast<AMyCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyCharacter_Sparrow* Sparrow = Cast<AMyCharacter_Sparrow>(UGameplayStatics::GetPlayerCharacter(this, 0));

	//TODO 캐릭터 인벤토리에 아이템 놓기 코드
	if (Greystone && Greystone->AddItemToInventory(this))
	{
		OnPickedUp();
	}

	else if (Countess && Countess->AddItemToInventory(this))
	{
		OnPickedUp();
	}

	else if (Sparrow && Sparrow->AddItemToInventory(this))
	{
		OnPickedUp();
	}
}

void APickup::Use_Implementation()
{
	GLog->Log("Use() from base pickup class : YOU SHOULD NOT BE SEEING THIS");

}

void APickup::OnPickedUp()
{
	MeshComp->SetVisibility(false);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void APickup::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter* Greystone = Cast<AMyCharacter>(OtherActor);
	AMyCharacter_Countess* Countess = Cast<AMyCharacter_Countess>(OtherActor);
	AMyCharacter_Sparrow* Sparrow = Cast<AMyCharacter_Sparrow>(OtherActor);
	
	if (Greystone)
	{
		Greystone->CheckForInteractable(this);		
	}
	
	else if (Countess)
	{
		Countess->CheckForInteractable(this);		
	}

	else if (Sparrow)
	{
		Sparrow->CheckForInteractable(this);
	}
}

