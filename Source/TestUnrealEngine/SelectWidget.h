// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectWidget.generated.h"


UCLASS()
class TESTUNREALENGINE_API USelectWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	//virtual void PostInitializeComponents();
	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void GreystonePressed();

	UFUNCTION()
	virtual void CountessPressed();

	UFUNCTION()
	virtual void SparrowPressed();

	UFUNCTION()
	virtual void SelectPressed();

	UFUNCTION()
	virtual void NextButtonPressed();

	UFUNCTION()
	virtual void BeforeButtonPressed();


private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Greystone;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Countess;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Sparrow;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectCharacter;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BeforeBtn;

	UPROPERTY()
	int32 ButtonIndex;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACharacterSelector> CurrentCharSelect;
	
	UPROPERTY()
	int32 NowCharacterIndex = 0;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;


};
