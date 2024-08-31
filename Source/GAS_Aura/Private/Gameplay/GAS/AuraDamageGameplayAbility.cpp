// Copyright fangh.space


#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraAbilityTypes.h"
#include "Untils/AuraLog.h"

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor) const
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

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	FDamageEffectParams Params;
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceASComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetASComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = Damage.GetValueAtLevel(GetAbilityLevel());
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DeBuffChance = DeBuffChance;
	Params.DeBuffDamage = DeBuffDamage;
	Params.DeBuffDuration = DeBuffDuration;
	Params.DeBuffFrequency = DeBuffFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockBackForceMagnitude = KnockBackForceMagnitude;
	Params.KnockBackChance = KnockBackChance;

	if (IsValid(TargetActor))
	{
		auto Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
		Rotation.Pitch = 45.f;

		const auto ToTarget = Rotation.Vector();
		Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		Params.KnockBackForce = ToTarget * KnockBackForceMagnitude;
	}
	
	return Params;
}
