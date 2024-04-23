// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemGlobals.h"
#include "Untils/AuraAbilityTypes.h"

FGameplayEffectContext* UAuraAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FAuraGameplayEffectContext{};
}
