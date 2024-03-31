// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemComponent.h"

#include "Untils/AuraLog.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::OnEffectApplied);
}

void UAuraAbilitySystemComponent::OnEffectApplied(UAbilitySystemComponent* ASComponent,
                                                  const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle)
{
	UE_LOG(Aura, Warning, TEXT("Effect Applied Self"));
}
