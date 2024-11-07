// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

class ULoadScreenSaveGame;
class USaveGame;
class UMVVM_LoadSlot;
class UDataAsset_AbilityInfo;
class UDataAsset_CharacterClassInfo;

UCLASS()
class GAS_AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura|Data|CharacterClassDefault")
	TObjectPtr<UDataAsset_CharacterClassInfo> DA_CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category="Aura|Data|AbilityInfo")
	TObjectPtr<UDataAsset_AbilityInfo> DA_AbilityInfo;

	// SaveGame
	// Slot
	UPROPERTY(EditDefaultsOnly, Category="Aura|SaveGame|Slot")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura|SaveGame")
	void SaveSlotData(const UMVVM_LoadSlot* LoadSlot, const int32 SlotIndex) const;

	UFUNCTION(BlueprintPure, Category="Aura|SaveGame")
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, const int32 SlotIndex) const;

	UFUNCTION(BlueprintCallable, Category="Aura|SaveGame")
	static void DeleteSlotData(const FString& SlotName, const int32 SlotIndex);
};
