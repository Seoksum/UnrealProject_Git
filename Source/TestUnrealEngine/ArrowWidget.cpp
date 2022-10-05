// Fill out your copyright notice in the Description page of Project Settings.


#include "ArrowWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "MyCharacter_Sparrow.h"


void UArrowWidget::BindArrowSpeed(class AMyCharacter_Sparrow* Sparrow)
{
	MySparrow = Sparrow;
	MySparrow->OnArrowSpeedChanged.AddUObject(this, &UArrowWidget::UpdateArrowSpeed);

}

void UArrowWidget::UpdateArrowSpeed()
{
	if (MySparrow != nullptr)
	{
		float NowRatio = (MySparrow->ArrowSpeedVal) / ArrowMaxValue;
		PB_Arrow->SetPercent(NowRatio);
	}
}
