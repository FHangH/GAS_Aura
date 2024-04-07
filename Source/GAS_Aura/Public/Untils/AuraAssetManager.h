// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "AuraAssetManager.generated.h"

UCLASS()
class GAS_AURA_API UAuraAssetManager : public UAssetManager
{
	GENERATED_BODY()

	/* Function */
public:
	static UAuraAssetManager& Get();

protected:
	virtual void StartInitialLoading() override;
};
