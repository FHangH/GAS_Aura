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
	const auto ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(DamageSpecHandle, DamageType, ScaledDamage);

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