// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstMapLevelScriptActor.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "OpeningDoor.h"

AFirstMapLevelScriptActor::AFirstMapLevelScriptActor()
{
	FStringAssetReference SequenceName(TEXT("LevelSequence'/Game/Cinematics/FirstMapSequence_G.FirstMapSequence_G'"));
	FirstMapSequence = Cast<ULevelSequence>(SequenceName.TryLoad());
}

void AFirstMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstanceRef)
	{
		if (FirstMapSequence && MyGameInstanceRef->GetPlayFirstMapSequence())
		{
			FMovieSceneSequencePlaybackSettings PlaybackSettings;
			ALevelSequenceActor* currentLevelSequenceActor = nullptr;

			SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
				GetWorld(),
				FirstMapSequence,
				PlaybackSettings,
				currentLevelSequenceActor);

			SequencePlayer->Play();

			FScriptDelegate funcDelegate;
			funcDelegate.BindUFunction(this, "DestroyDoor");

			SequencePlayer->OnFinished.AddUnique(funcDelegate);

			TArray<AActor*> Portals;
			UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Portal"), Portals);
			for (auto portal : Portals)
			{
				portal->SetActorHiddenInGame(true);
			}
		}
	}
}

void AFirstMapLevelScriptActor::DestroyDoor()
{
	TArray<AActor*> Doors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindDoorType, Doors);
	for (AActor* actor : Doors)
	{
		actor->Destroy();
	}

	GetWorld()->GetTimerManager().ClearTimer(DoorTimerHandle);

}
