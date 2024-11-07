// Copyright fangh.space


#include "UI/HUD/LoadScreenHUD.h"
#include "Kismet/GameplayStatics.h"
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

	// Load Slot Data To MVVM
	if (MVVM_LoadScreen)
	{
		MVVM_LoadScreen->LoadData();
	}

	SetUserInputMode();
}

void ALoadScreenHUD::SetUserInputMode() const
{
	const auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	FInputModeUIOnly InputMode_OnlyUI;
	InputMode_OnlyUI.SetWidgetToFocus(LoadScreenWidget->TakeWidget());
	InputMode_OnlyUI.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->bShowMouseCursor = true;
	PC->SetInputMode(InputMode_OnlyUI);
}
 