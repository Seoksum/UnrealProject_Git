// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TESTUNREALENGINE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	enum EHUDState : uint8
	{
		HS_Ingame,
		HS_Inventory,
		HS_Restart,
	};
	void ApplyHUDChanges();	//hud state를 체크하고,우리가 원하는 hud state로 적용(바꾸기)하기위한 함수
	uint8 GetHUDState(); // hudstate 받아오는 함수

	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState); // 새로운 hudstate를 받아온 후 applyHUDChanges 호출

	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents); // screen에 hud를 적용한 후 성공여부 반환

	

public:

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY()
	UUserWidget* RestartWidget;

	uint8 HUDState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> IngameHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> InventoryHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<class UUserWidget> RestartHUDClass;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;
	
	
};
