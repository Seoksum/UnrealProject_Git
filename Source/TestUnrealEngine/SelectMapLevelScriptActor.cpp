// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMapLevelScriptActor.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequence.h"
#include "MyCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.h"
#include "Fountain.h"
#include "TimerManager.h"



ASelectMapLevelScriptActor::ASelectMapLevelScriptActor()
{

	FStringAssetReference SequenceName(TEXT("LevelSequence'/Game/Cinematics/FirstMapSequence_G.FirstMapSequence_G'"));
	FirstMapSequence = Cast<ULevelSequence>(SequenceName.TryLoad());


}

void ASelectMapLevelScriptActor::BeginPlay()
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

void ASelectMapLevelScriptActor::DestroyDoor()
{
	TArray<AActor*> Doors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FindDoorType, Doors);
	for (AActor* actor : Doors)
	{
		actor->Destroy();
	}

	GetWorld()->GetTimerManager().ClearTimer(DoorTimerHandle);

}



