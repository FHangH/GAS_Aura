// Copyright fangh.space


#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "GameFramework/SaveGame.h"
#include "Gameplay/SaveGame/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MVVM/MVVM_LoadSlot.h"

void AAuraGameModeBase::SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const
{
	if (!IsValid(LoadScreenSaveGameClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("LoadScreenSaveGameClass is null"));
		return;
	}
	
	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
	}
	const auto LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass));
	LoadScreenSaveGame->PlayerName = LoadSlot->PlayerName;
	LoadScreenSaveGame->SlotStatus = ESSS_Taken;
		
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	const auto LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}
