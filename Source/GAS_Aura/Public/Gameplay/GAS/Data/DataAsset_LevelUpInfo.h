// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DataAsset_LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement { 0 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointAward { 1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointAward { 1 };
};

UCLASS(BlueprintType)
class GAS_AURA_API UDataAsset_LevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	/* Function */
public:
	int32 FindLevelForXP(const int32 XP) const;
};
