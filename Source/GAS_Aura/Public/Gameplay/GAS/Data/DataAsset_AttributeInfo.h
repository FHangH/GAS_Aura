// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "DataAsset_AttributeInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag AttributeTag {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeName {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText AttributeDescription {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float AttributeValue {0.f};
};

UCLASS(BlueprintType)
class GAS_AURA_API UDataAsset_AttributeInfo : public UDataAsset
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TArray<FAuraAttributeInfo> AttributeInformation;

	/* Function */
public:
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound = false) const;
};
