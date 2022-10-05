// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FirstMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UFirstMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetFirstMsg();

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InstructionText;



};
