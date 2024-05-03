// Copyright fangh.space


#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Untils/AuraLog.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	if (!TargetActor || !DamageEffectClass)
	{
		UE_LOG(Aura, Warning, TEXT("TargetActor is Null Or DamageEffectClass is not set for %s"), *GetName());
		return;
	}
	
	const auto DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, GetAbilityLevel());
	for (const auto& Pair : DamageTypes)
	{
		const auto ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, Pair.Key, ScaledDamage);
	}

	if (const auto ASC = GetAbilitySystemComponentFromActorInfo())
	{
		const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!TargetASC) return;
		
		ASC->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data.Get(), TargetASC);
	}
}
