// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyCharacterrWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyCharacterrWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHp(class UMyStatComponent* StatComp);
	void UpdateHp();

	void BindMana(class UMyStatComponent* StatComp);
	void UpdateMana();



	
private:
	TWeakObjectPtr<class UMyStatComponent> CurrentStatComp;
	TWeakObjectPtr<class UMyGameInstance> MyGameInstanceRef;

	UPROPERTY(meta=(BindWidget))
	class UProgressBar* PB_HpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_ManaBar;
};
