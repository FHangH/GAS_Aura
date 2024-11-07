// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

UCLASS()
class GAS_AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString SlotName {};

	UPROPERTY(BlueprintReadWrite, Category="SaveGame|LoadScreen")
	int32 SlotIndex { 0 };

	UPROPERTY(BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString PlayerName { "Default Name" };
};
