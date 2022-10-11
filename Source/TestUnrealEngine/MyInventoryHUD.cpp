// Fill out your copyright notice in the Description page of Project Settings.


#include "MyInventoryHUD.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "MyGameInstance.h"
#include "MyStatComponent.h"


void UMyInventoryHUD::NativeOnInitialized()
{
	UWorld* World = GetWorld();
	/*if (GetWorld())
	{
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}*/

	slot_btn1->OnClicked.AddDynamic(this, &UMyInventoryHUD::PressedSlot1);
	slot_btn2->OnClicked.AddDynamic(this, &UMyInventoryHUD::PressedSlot2);

}

void UMyInventoryHUD::PressedSlot1()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstanceRef)
	{
		if (MyGameInstanceRef->GetHpCount() < 1)
			return;
		int hp = MyGameInstanceRef->GetNowHp();
		MyGameInstanceRef->SetNowHp(hp + 10);
		auto Stat = MyGameInstanceRef->GetStatComp();
		Stat->SetHp(hp + 10);

		int hp_cnt = MyGameInstanceRef->GetHpCount();
		hp_cnt -= 1;
		MyGameInstanceRef->SetHpCount(hp_cnt);
		if (hp_cnt != 0)
			SetSlot1(hp_cnt);
		else
			SetZeroValueSlot1();
	}
}

void UMyInventoryHUD::PressedSlot2()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstanceRef)
	{
		if (MyGameInstanceRef->GetManaCount() < 1)
			return;
		int mana = MyGameInstanceRef->GetNowMana();
		MyGameInstanceRef->SetNowMana(mana + 10);
		auto Stat = MyGameInstanceRef->GetStatComp();
		Stat->SetMana(mana + 10);

		int mana_cnt = MyGameInstanceRef->GetManaCount();
		mana_cnt -= 1;
		MyGameInstanceRef->SetManaCount(mana_cnt);
		if (mana_cnt != 0)
			SetSlot2(mana_cnt);
		else
			SetZeroValueSlot2();

	}
}

void UMyInventoryHUD::SetSlot1(int32 HpCount)
{
	if (HpCount >= 1)
	{
		const FString HPpath = "Texture2D'/Game/Images/Hp_Thumbnail.Hp_Thumbnail'";
		UTexture2D* HpTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *HPpath));
		slot_img1->SetBrushFromTexture(HpTexture);
		const FString Hp_Str = FString::Printf(TEXT("%d"), HpCount);
		slot_txt1->SetText(FText::FromString(Hp_Str));
	}	
}

void UMyInventoryHUD::SetSlot2(int32 ManaCount)
{
	if (ManaCount >= 1)
	{
		const FString HPpath = "Texture2D'/Game/Images/Mana_Thumbnail.Mana_Thumbnail'";
		UTexture2D* ManaTexture = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *HPpath));
		slot_img2->SetBrushFromTexture(ManaTexture);
		const FString Mana_Str = FString::Printf(TEXT("%d"), ManaCount);
		slot_txt2->SetText(FText::FromString(Mana_Str));
	}	
	
}

void UMyInventoryHUD::SetZeroValueSlot1()
{
	const FString Hp_Str = FString::Printf(TEXT("%d"), 0);
	slot_txt1->SetText(FText::FromString(Hp_Str));
}

void UMyInventoryHUD::SetZeroValueSlot2()
{
	const FString Mana_Str = FString::Printf(TEXT("%d"), 0);
	slot_txt2->SetText(FText::FromString(Mana_Str));
}
