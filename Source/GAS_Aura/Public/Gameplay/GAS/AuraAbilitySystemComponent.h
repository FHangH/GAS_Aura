// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Function */
public:
	void AbilityActorInfoSet();
	
protected:
	void OnEffectApplied(UAbilitySystemComponent* ASComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle);
};
