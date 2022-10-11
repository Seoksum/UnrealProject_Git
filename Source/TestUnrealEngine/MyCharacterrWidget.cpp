// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterrWidget.h"
#include "MyStatComponent.h"
#include "Components/ProgressBar.h"
#include "MyGameInstance.h"




void UMyCharacterrWidget::BindHp(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;

	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());

	CurrentStatComp->OnHpChanged.AddUObject(this, &UMyCharacterrWidget::UpdateHp);

}

void UMyCharacterrWidget::UpdateHp()
{
	if(CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
}

void UMyCharacterrWidget::BindMana(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;

	if (CurrentStatComp.IsValid())
		PB_ManaBar->SetPercent(CurrentStatComp->GetManaRatio());

	CurrentStatComp->OnManaChanged.AddUObject(this, &UMyCharacterrWidget::UpdateMana);

}

void UMyCharacterrWidget::UpdateMana()
{
	if (CurrentStatComp.IsValid())
		PB_ManaBar->SetPercent(CurrentStatComp->GetManaRatio());
}

