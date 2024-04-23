// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "AuraAbilitySystemGlobals.generated.h"

UCLASS()
class GAS_AURA_API UAuraAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	/* Function */
public:
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
