// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyInventoryHUD.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API UMyInventoryHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void PressedSlot1();

	UFUNCTION()
	virtual void PressedSlot2();

	
public:

	void SetSlot1(int32 HpCount);
	void SetSlot2(int32 ManaCount);

	void SetZeroValueSlot1();
	void SetZeroValueSlot2();


	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;


private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* InventoryTxt;

	UPROPERTY(meta = (BindWidget))
	class UButton* slot_btn1;

	UPROPERTY(meta = (BindWidget))
	class UButton* slot_btn2;

	UPROPERTY(meta = (BindWidget))
	class UButton* slot_btn3;

	UPROPERTY(meta = (BindWidget))
	class UButton* slot_btn4;

	UPROPERTY(meta = (BindWidget))
	class UImage* slot_img1;

	UPROPERTY(meta = (BindWidget))
	class UImage* slot_img2;

	UPROPERTY(meta = (BindWidget))
	class UImage* slot_img3;

	UPROPERTY(meta = (BindWidget))
	class UImage* slot_img4;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* slot_txt1;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* slot_txt2;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* slot_txt3;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* slot_txt4;

	

};
