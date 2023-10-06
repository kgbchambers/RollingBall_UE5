// Fill out your copyright notice in the Description page of Project Settings.
#include "RollABallWidget.h"
#include "RollABallGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "RollABall/Items/RollABallItemBase.h"
#include "Macros.h"

void ARollABallGameModeBase::BeginPlay()
{
	TArray<AActor*> Items;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARollABallItemBase::StaticClass(), Items);
	ItemsInLevel = Items.Num();

	if (GameWidgetClass)
	{
		GameWidget = Cast<URollABallWidget>(CreateWidget(GetWorld(), GameWidgetClass));

		if(GameWidget)
		{
			GameWidget->AddToViewport();
			UpdateItemText();
		}
	}
}

void ARollABallGameModeBase::UpdateItemText()
{
	GameWidget->SetItemText(ItemsCollected, ItemsInLevel);
}

void ARollABallGameModeBase::ResetCount()
{
	ItemsCollected = 0;
	UpdateItemText();
	ResetBPEvent();
}


void ARollABallGameModeBase::ItemCollected()
{
	ItemsCollected++;
	UpdateItemText();

	if (ItemsCollected >= ItemsInLevel)
	{
		ResetCount();
		D("Resetting Items");
	}
}

