// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UENUM(BlueprintType)
enum ESaveSlotStatus : uint8
{
	ESSS_Vacant		UMETA(DisplayName="Vacant"),
	ESSS_EnterName	UMETA(DisplayName="EnterName"),
	ESSS_Taken		UMETA(DisplayName="Taken"),
};

UCLASS()
class GAS_AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString SlotName {};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	int32 SlotIndex { 0 };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString PlayerName { "Default Name" };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString MapName { "Default Map" };
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	TEnumAsByte<ESaveSlotStatus> SlotStatus { ESaveSlotStatus::ESSS_Vacant };
};
