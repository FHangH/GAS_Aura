// Copyright fangh.space


#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/SaveGame.h"
#include "Gameplay/GameInstance/AuraGameInstance.h"
#include "Gameplay/SaveGame/LoadScreenSaveGame.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/MVVM/MVVM_LoadSlot.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	Map_Levels.Add(DefaultMapName, DefaultMap);
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	const auto AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);

	if (Actors.Num() > 0)
	{
		auto SelectedActor = Actors[0];
		for (const auto& Actor : Actors)
		{
			if (!AuraGameInstance) break;
			if (const auto PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

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
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
	LoadScreenSaveGame->SlotStatus = ESSS_Taken;
	LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
		
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

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData() const
{
	if (const auto AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const auto InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const auto InGameSlotIndex = AuraGameInstance->LoadSlotIndex;
		return GetSaveSlotData(InGameLoadSlotName, InGameSlotIndex);
	}
	return nullptr;
}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (const auto& Map : Map_Levels)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return {};
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject) const
{
	if (const auto AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance()))
	{
		const auto InGameLoadSlotName = AuraGameInstance->LoadSlotName;
		const auto InGameSlotIndex = AuraGameInstance->LoadSlotIndex;
		AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;
		UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameSlotIndex);
	}
}

void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
{
	if (!World) return;
	auto WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const auto AuraGIS = Cast<UAuraGameInstance>(GetGameInstance());
	if (!AuraGIS) return;

	if (auto SaveGame = GetSaveSlotData(AuraGIS->LoadSlotName, AuraGIS->LoadSlotIndex))
	{
		if (!DestinationMapAssetName.IsEmpty())
		{
			SaveGame->MapAssetName = DestinationMapAssetName;
			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
		}
		
		if (!SaveGame->HasMap(WorldName))
		{
			FSavedMap NewSavedMap;
			NewSavedMap.MapAssetName = WorldName;
			SaveGame->SavedMaps.Add(NewSavedMap);
		}
		auto SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
		SavedMap.SavedActors.Empty();

		for (FActorIterator It(World); It; ++It)
		{
			const auto Actor = *It;
			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;

			FSavedActor SavedActor;
			SavedActor.ActorName = Actor->GetFName();
			SavedActor.Transform = Actor->GetTransform();

			FMemoryWriter MemoryWriter(SavedActor.Bytes);
			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
			Archive.ArIsSaveGame = true;
			Actor->Serialize(Archive);
			SavedMap.SavedActors.AddUnique(SavedActor);
		}

		for (auto& MapToReplace : SaveGame->SavedMaps)
		{
			if (MapToReplace.MapAssetName == WorldName)
			{
				MapToReplace = SavedMap;
			}
		}
		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGIS->LoadSlotName, AuraGIS->LoadSlotIndex);
	}
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	if (!World) return;
	auto WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	const auto AuraGIS = Cast<UAuraGameInstance>(GetGameInstance());
	if (!AuraGIS) return;

	if (UGameplayStatics::DoesSaveGameExist(AuraGIS->LoadSlotName, AuraGIS->LoadSlotIndex))
	{
		const auto SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGIS->LoadSlotName, AuraGIS->LoadSlotIndex));
		if (!SaveGame) return;
		
		for (FActorIterator It(World); It; ++It)
		{
			const auto Actor = *It;
			if (!Actor->Implements<USaveInterface>()) continue;

			for (const auto& SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}
					FMemoryReader MemoryReader(SavedActor.Bytes);
					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive);
					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

void AAuraGameModeBase::DeleteSlotData(const FString& SlotName, const int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Map_Levels.FindChecked(Slot->GetMapName()));
}
