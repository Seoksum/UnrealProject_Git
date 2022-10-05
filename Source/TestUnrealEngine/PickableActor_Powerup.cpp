// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableActor_Powerup.h"

// Sets default values
APickableActor_Powerup::APickableActor_Powerup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickableActor_Powerup::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APickableActor_Powerup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

