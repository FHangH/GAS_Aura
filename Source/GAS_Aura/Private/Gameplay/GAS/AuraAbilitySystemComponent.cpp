// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraGameplayAbility.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::Client_OnEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const auto& GAClass : StartUpAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{GAClass, 1};
		
		if (const auto AuraGA = Cast<UAuraGameplayAbility>(GASpec.Ability))
		{
			GASpec.DynamicAbilityTags.AddTag(AuraGA->StartUpInputTag);
			GiveAbility(GASpec);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		AbilitySpecInputPressed(GASpec);
		if (!GASpec.IsActive())
		{
			TryActivateAbility(GASpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		AbilitySpecInputReleased(GASpec);
	}
}

void UAuraAbilitySystemComponent::Client_OnEffectApplied_Implementation(UAbilitySystemComponent* ASComponent,
	const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTagDelegate.Broadcast(TagContainer);
}