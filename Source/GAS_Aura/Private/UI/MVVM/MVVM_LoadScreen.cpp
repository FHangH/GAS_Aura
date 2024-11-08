// Copyright fangh.space


#include "UI/MVVM/MVVM_LoadScreen.h"
#include "Gameplay/GameInstance/AuraGameInstance.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Gameplay/SaveGame/LoadScreenSaveGame.h"
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

		LoadSlot_0->SlotIndex = 0;
		LoadSlot_1->SlotIndex = 1;
		LoadSlot_2->SlotIndex = 2;
		
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
		Map_LoadSlots[Slot]->SetMapName(AuraGameMode->DefaultMapName);
		Map_LoadSlots[Slot]->SlotStatus = ESSS_Taken;
		Map_LoadSlots[Slot]->InitializeSlot();
		AuraGameMode->SaveSlotData(Map_LoadSlots[Slot], Slot);

		if (const auto AuraGameInstance = Cast<UAuraGameInstance>(AuraGameMode->GetGameInstance()))
		{
			AuraGameInstance->LoadSlotName = Map_LoadSlots[Slot]->LoadSlotName;
			AuraGameInstance->LoadSlotIndex = Map_LoadSlots[Slot]->SlotIndex;
			AuraGameInstance->PlayerStartTag = AuraGameMode->DefaultPlayerStartTag;
		}
	}
}

void UMVVM_LoadScreen::NewGameButtonPressed(const int32 Slot)
{
	Map_LoadSlots[Slot]->SetWidgetSwitcherIndexDelegate.Broadcast(1);
}

void UMVVM_LoadScreen::SelectSlotButtonPressed(const int32 Slot)
{
	SlotSelectedDelegate.Broadcast();
	
	for (const auto& LoadSlot : Map_LoadSlots)
	{
		LoadSlot.Value->EnableSelectSlotButtonDelegate.Broadcast(LoadSlot.Key != Slot);
	}
	SelectedSlot = Map_LoadSlots[Slot];
}

void UMVVM_LoadScreen::DeleteSlotButtonPressed() const
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode || !SelectedSlot) return;

	AuraGameMode->DeleteSlotData(SelectedSlot->LoadSlotName, SelectedSlot->SlotIndex);
	SelectedSlot->SlotStatus = ESSS_Vacant;
	SelectedSlot->InitializeSlot();
	SelectedSlot->EnableSelectSlotButtonDelegate.Broadcast(true);
}

void UMVVM_LoadScreen::PlayButtonPressed() const
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode || !SelectedSlot) return;

	AuraGameMode->TravelToMap(SelectedSlot);
}

void UMVVM_LoadScreen::LoadData()
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (!AuraGameMode) return;

	for (const auto& LoadSlot : Map_LoadSlots)
	{
		if (const auto SaveObject = AuraGameMode->GetSaveSlotData(LoadSlot.Value->LoadSlotName, LoadSlot.Key))
		{
			LoadSlot.Value->SetPlayerName(SaveObject->PlayerName);
			LoadSlot.Value->SlotStatus = SaveObject->SlotStatus;
			LoadSlot.Value->SetMapName(SaveObject->MapName);
			LoadSlot.Value->InitializeSlot();
		}
	}
}
