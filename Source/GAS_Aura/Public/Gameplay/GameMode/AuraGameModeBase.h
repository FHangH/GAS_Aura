// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AuraGameModeBase.generated.h"

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
};
