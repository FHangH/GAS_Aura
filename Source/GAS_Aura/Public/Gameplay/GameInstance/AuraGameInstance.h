// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuraGameInstance.generated.h"

UCLASS()
class GAS_AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aura|GIS")
	FName PlayerStartTag {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aura|GIS")
	FString LoadSlotName {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aura|GIS")
	int32 LoadSlotIndex { 0 };
};
