// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagDelegate, const FGameplayTagContainer& /*AssetTags*/)

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Property */
public:
	FEffectAssetTagDelegate EffectAssetTagDelegate;

	/* Function */
public:
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	
protected:
	void OnEffectApplied(UAbilitySystemComponent* ASComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const;
};
