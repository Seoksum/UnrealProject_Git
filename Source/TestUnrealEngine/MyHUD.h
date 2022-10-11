// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UMyHUD(const FObjectInitializer& ObjectInitializer);

	void SetGreystone();
	void SetCountess();
	void SetSparrow();

	void SetLevel();

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* LevelText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Level;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Q;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_E;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_R;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Q;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_E;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_R;

	

};
