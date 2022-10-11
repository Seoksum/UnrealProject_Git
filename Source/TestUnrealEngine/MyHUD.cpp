// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"


UMyHUD::UMyHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{	
	//SetGreystone();
}

void UMyHUD::SetGreystone()
{
	SetLevel();

	const FString Q_Path = "Texture2D'/Game/Images/Greystone_Q.Greystone_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Greystone_E.Greystone_E'";
	const FString R_Path = "Texture2D'/Game/Images/Greystone_R.Greystone_R'";
	const FString CharacterThumbnail = "Texture2D'/Game/Images/Greystone_Image.Greystone_Image'";

	UTexture2D* MyTexture_Q = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Q_Path));
	UTexture2D* MyTexture_E = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *E_Path));
	UTexture2D* MyTexture_R = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *R_Path));
	UTexture2D* MyImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *CharacterThumbnail));

	Image_Q->SetBrushFromTexture(MyTexture_Q);
	Image_E->SetBrushFromTexture(MyTexture_E);
	Image_R->SetBrushFromTexture(MyTexture_R);


}

void UMyHUD::SetCountess()
{
	SetLevel();

	const FString Q_Path = "Texture2D'/Game/Images/Countess_Q.Countess_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Countess_E.Countess_E'";
	const FString R_Path = "Texture2D'/Game/Images/Countess_R.Countess_R'";
	const FString CharacterThumbnail = "Texture2D'/Game/Images/Countess_Image.Countess_Image'";

	UTexture2D* MyTexture_Q = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Q_Path));
	UTexture2D* MyTexture_E = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *E_Path));
	UTexture2D* MyTexture_R = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *R_Path));
	UTexture2D* MyImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *CharacterThumbnail));
	
	Image_Q->SetBrushFromTexture(MyTexture_Q);
	Image_E->SetBrushFromTexture(MyTexture_E);
	Image_R->SetBrushFromTexture(MyTexture_R);

}

void UMyHUD::SetSparrow()
{
	SetLevel();

	const FString Q_Path = "Texture2D'/Game/Images/Sparrow_Q.Sparrow_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Sparrow_E.Sparrow_E'";
	const FString R_Path = "Texture2D'/Game/Images/Sparrow_R.Sparrow_R'";
	const FString CharacterThumbnail = "Texture2D'/Game/Images/Sparrow_Image.Sparrow_Image'";

	UTexture2D* MyTexture_Q = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Q_Path));
	UTexture2D* MyTexture_E = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *E_Path));
	UTexture2D* MyTexture_R = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *R_Path));
	UTexture2D* MyImage = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *CharacterThumbnail));

	Image_Q->SetBrushFromTexture(MyTexture_Q);
	Image_E->SetBrushFromTexture(MyTexture_E);
	Image_R->SetBrushFromTexture(MyTexture_R);

}

void UMyHUD::SetLevel()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		const FString MyLevel = FString::Printf(TEXT("%d"),MyGameInstanceRef->GetNowLevel());
		Level->SetText(FText::FromString(MyLevel));
	}
}