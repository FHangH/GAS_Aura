// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const auto& Item : StartUpAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{Item};
		//GiveAbility(GASpec);
		GiveAbilityAndActivateOnce(GASpec);
	}
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* ASComponent,
                                                  const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagDelegate.Broadcast(TagContainer);
}
