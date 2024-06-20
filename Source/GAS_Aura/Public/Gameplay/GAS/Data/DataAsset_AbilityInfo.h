// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AbilityInfo.generated.h"

class UGameplayAbility;

USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AbilityTag {};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FGameplayTag InputTag {};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	FGameplayTag StatusTag {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag CooldownTag {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UTexture2D> Icon {};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelRequirement { 1 };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability;
};

UCLASS(BlueprintType)
class GAS_AURA_API UDataAsset_AbilityInfo : public UDataAsset
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TArray<FAuraAbilityInfo> AbilityInformation;
	
	/* Function */
public:
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag, const bool bLogNotFound = false) const;
};
