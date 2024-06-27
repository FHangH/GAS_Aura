// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

UCLASS()
class GAS_AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FGameplayTag StartUpInputTag;

	/* Function */
public:
	virtual FString GetDescription(const int32 Level);
	virtual FString GetNextLevelDescription(const int32 Level);
	static FString GetLockedDescription(const int32 Level);
};
