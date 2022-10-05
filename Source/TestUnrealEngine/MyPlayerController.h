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
	void ApplyHUDChanges();	//hud state�� üũ�ϰ�,�츮�� ���ϴ� hud state�� ����(�ٲٱ�)�ϱ����� �Լ�
	uint8 GetHUDState(); // hudstate �޾ƿ��� �Լ�

	UFUNCTION(BlueprintCallable, Category = "HUD Functions")
	void ChangeHUDState(uint8 NewState); // ���ο� hudstate�� �޾ƿ� �� applyHUDChanges ȣ��

	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents); // screen�� hud�� ������ �� �������� ��ȯ

	

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
