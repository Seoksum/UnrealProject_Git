// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy_Buff_White.h"
#include "MyCharacter_Sparrow.h"
#include "MyStatComponent.h"


// Sets default values
AWeapon_Arrow::AWeapon_Arrow()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> DM(TEXT("StaticMesh'/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));
	if (DM.Succeeded())
		Mesh->SetStaticMesh(DM.Object);


	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionComp->SetBoxExtent(FVector(60.f, 40.f, 20.f));
	CollisionComp->SetCollisionProfileName(FName("Arrow"));
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetEnableGravity(true);

	CollisionComp->SetNotifyRigidBodyCollision(false);
	CollisionComp->OnComponentHit.AddDynamic(this, &AWeapon_Arrow::OnHit);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeapon_Arrow::OnBeginOverlap);

	RootComponent = CollisionComp;
	Mesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->ProjectileGravityScale = 1.f;
	//ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = false;
	ProjectileMovement->bInitialVelocityInLocalSpace = true;


	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	SetActorLocation(FVector(0.f, 0.f, 30.f));

	InitialLifeSpan = 3.f;

}

// Called when the game starts or when spawned
void AWeapon_Arrow::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon_Arrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AWeapon_Arrow::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{


}

void AWeapon_Arrow::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMyCharacter_Sparrow* MyCharacter = Cast<AMyCharacter_Sparrow>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	UMyStatComponent* Stat = MyCharacter->Stat;

	FDamageEvent DamageEvent;
	FTransform EffectTrasform(FRotator::ZeroRotator, OtherActor->GetActorLocation());

	if (MyCharacter->IsAttacking)
		OtherActor->TakeDamage(Stat->GetAttack(), DamageEvent, MyCharacter->GetController(), this);

	else if (MyCharacter->IsAttackingQ)
	{
		OtherActor->TakeDamage(Stat->GetAttack_Q(), DamageEvent, MyCharacter->GetController(), this);		
		if (SkillEffectQ)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffectQ, EffectTrasform, true);
	}
	else if (MyCharacter->IsAttackingE)
	{
		OtherActor->TakeDamage(Stat->GetAttack_E(), DamageEvent, MyCharacter->GetController(), this);
		if (SkillEffectE)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffectE, EffectTrasform, true);
	}
	else if (MyCharacter->IsAttackingR)
	{
		OtherActor->TakeDamage(Stat->GetAttack_R(), DamageEvent, MyCharacter->GetController(), this);
		if (SkillEffectR)
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffectR, EffectTrasform, true);
	}
	this->Destroy();

}




