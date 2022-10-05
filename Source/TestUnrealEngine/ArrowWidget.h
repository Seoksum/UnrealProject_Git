// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArrowWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UArrowWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:	
	void BindArrowSpeed(class AMyCharacter_Sparrow* Sparrow);
	void UpdateArrowSpeed();

private:

	UPROPERTY()
	float ArrowMaxValue = 2200.f;

	TWeakObjectPtr<class AMyCharacter_Sparrow> MySparrow;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Arrow;
};
