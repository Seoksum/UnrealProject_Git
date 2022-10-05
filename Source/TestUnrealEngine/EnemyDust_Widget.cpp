// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyDust_Widget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UEnemyDust_Widget::BindHp(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UEnemyDust_Widget::UpdateHp);
}

void UEnemyDust_Widget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetEnemyHpRatio());
}

void UEnemyDust_Widget::BindLevel(int32 Level)
{
	const FString MyLevel = FString::Printf(TEXT("%d"), Level);
	LevelText->SetText(FText::FromString(MyLevel));
}



