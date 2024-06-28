// Copyright fangh.space


#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
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

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages)
{
	if (TaggedMontages.IsEmpty()) return FTaggedMontage{};
	return TaggedMontages[FMath::RandRange(0, TaggedMontages.Num() - 1)];
}

float UAuraDamageGameplayAbility::GetDamageByDamageType(const float InLevel, const FGameplayTag& DamageType)
{
	checkf(DamageTypes.Contains(DamageType), TEXT("GameplayAbility [%s] does not contain DamageType [%s]"), *GetNameSafe(this), *DamageType.ToString());
	return DamageTypes[DamageType].GetValueAtLevel(InLevel);
}
