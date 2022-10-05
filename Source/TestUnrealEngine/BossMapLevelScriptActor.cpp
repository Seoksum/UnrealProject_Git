// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMapLevelScriptActor.h"
#include "MovieSceneSequencePlayer.h"
#include "LevelSequence/Public/LevelSequencePlayer.h"
#include "LevelSequence/Public/LevelSequence.h"



ABossMapLevelScriptActor::ABossMapLevelScriptActor()
{
	FStringAssetReference SequenceName(TEXT("LevelSequence'/Game/Cinematics/BossAppearSequence.BossAppearSequence'"));
	BossMapSequence = Cast<ULevelSequence>(SequenceName.TryLoad());
}

void ABossMapLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();
	if (BossMapSequence)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		ALevelSequenceActor* currentLevelSequenceActor = nullptr;

		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			GetWorld(),
			BossMapSequence,
			PlaybackSettings,
			currentLevelSequenceActor
		);

		//SequencePlayer->Play();
	}

}
