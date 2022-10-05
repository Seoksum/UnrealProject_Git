// Fill out your copyright notice in the Description page of Project Settings.


#include "MyWeapon.h"
#include "MyCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AMyWeapon::AMyWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WEAPON"));
	Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("TRIGGER"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SW(TEXT("StaticMesh'/Game/ParagonGreystone/FX/Meshes/Heroes/Greystone/SM_Greystone_Blade_01.SM_Greystone_Blade_01'"));

	if (SW.Succeeded())
	{
		Weapon->SetStaticMesh(SW.Object);
	}

	Weapon->SetupAttachment(RootComponent);
	Trigger->SetupAttachment(Weapon); //Trigger ������Ʈ�� ���� ������Ʈ�� ����ٳ�� �Ѵ�. 

	Weapon->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetCollisionProfileName(TEXT("MyCollectible"));
	Trigger->SetBoxExtent(FVector(30.f, 30.f, 30.f));
	// Ʈ���� �ڽ��� ũ�⸦ (30,30,30)���� �س��´�. (�浹 ����)

	
}

// Called when the game starts or when spawned
void AMyWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(4.f, 4.f, 4.f));
}

void AMyWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyWeapon::OnCharacterOverlap);
	
}

void AMyWeapon::OnCharacterOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Overlapped"));

	AMyCharacter* MyCharacter = Cast<AMyCharacter>(OtherActor);

	if (MyCharacter)
	{
		FName WeaponSocket(TEXT("hand_l_socket"));

		AttachToComponent(MyCharacter->GetMesh(),
			FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			WeaponSocket);
	}
}



