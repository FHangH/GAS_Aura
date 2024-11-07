// Copyright fangh.space


#include "UI/MVVM/MVVM_LoadScreen.h"

#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	if (MVVM_LoadSlotClass)
	{
		LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		
		LoadSlot_0->LoadSlotName = FString{ "LoadSlot_0" };
		LoadSlot_1->LoadSlotName = FString{ "LoadSlot_1" };
		LoadSlot_2->LoadSlotName = FString{ "LoadSlot_2" };
		
		Map_LoadSlots.Add(0, LoadSlot_0);
		Map_LoadSlots.Add(1, LoadSlot_1);
		Map_LoadSlots.Add(2, LoadSlot_2);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MVVM_LoadSlotClass is null"));
	}
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetMVVM_LoadSlotByIndex(const int32 Index) const
{
	return Map_LoadSlots.FindChecked(Index);
}

void UMVVM_LoadScreen::NewSlotButtonPressed(const int32 Slot, const FString& EnteredName)
{
	if (const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		Map_LoadSlots[Slot]->SetPlayerName(EnteredName);
		AuraGameMode->SaveSlotData(Map_LoadSlots[Slot], Slot);
		Map_LoadSlots[Slot]->InitializeSlot();
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(const int32 Slot)
{
	Map_LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(const int32 Slot)
{
}
