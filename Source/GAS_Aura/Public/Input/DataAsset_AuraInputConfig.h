// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AuraInputConfig.generated.h"

struct FGameplayTag;
class UInputAction;

USTRUCT(BlueprintType)
struct FAuraInputAction
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	const UInputAction* InputAction { nullptr };

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FGameplayTag InputTag {};
};

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UDataAsset_AuraInputConfig : public UDataAsset
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TArray<FAuraInputAction> AbilityInputActions;

	/* Function */
public:
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound = false) const;
};
