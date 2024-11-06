﻿// Copyright fangh.space


#include "UI/HUD/LoadScreenHUD.h"
#include "UI/MVVM/MVVM_LoadScreen.h"
#include "UI/Widget/LoadScreenWidget.h"

void ALoadScreenHUD::BeginPlay()
{
	Super::BeginPlay();

	// Create LoadScreen MVVM
	if (MVVM_LoadScreenClass)
	{
		MVVM_LoadScreen = NewObject<UMVVM_LoadScreen>(this, MVVM_LoadScreenClass);
		MVVM_LoadScreen->InitializeLoadSlots();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MVVM_LoadScreenClass is null"));
	}

	// Create LoadScreen Widget
	if (LoadScreenWidgetClass)
	{
		LoadScreenWidget = CreateWidget<ULoadScreenWidget>(GetWorld(), LoadScreenWidgetClass);
		if (LoadScreenWidget)
		{
			LoadScreenWidget->AddToViewport();
			LoadScreenWidget->BlueprintInitWidget();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadScreenWidgetClass is null"));
	}
}
 