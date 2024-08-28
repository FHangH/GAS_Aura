// Copyright fangh.space


#include "Gameplay/GAS/ExecCalc/EC_Damage.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArcaneResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
	}
};

static const AuraDamageStatics& DamageStatics()
{
	static AuraDamageStatics Statics;
	return Statics;
}

UEC_Damage::UEC_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
}

void UEC_Damage::DetermineDeBuff(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	const FGameplayEffectSpec& GESpec,
	const FAggregatorEvaluateParameters& EvaluateParams,
	const TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition>& InTagsToDefs)
{
	const auto GameplayTags = FAuraGameplayTags::Get();
	for (auto Pair : GameplayTags.DamageTypesToDeBuffs)
	{
		const auto DamageType = Pair.Key;
		const auto DeBuffType = Pair.Value;
		const auto TypeDamage = GESpec.GetSetByCallerMagnitude(DamageType, false, -1.f);

		if (TypeDamage > -0.5f) // 0.5 padding for floating point [im]precision
		{
			// Determine if there was a successful DeBuff
			const auto SourceDeBuffChance =
				GESpec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Chance, false, -1.f);

			auto TargetDeBuffResistance { 0.f };
			const auto ResistanceTag = GameplayTags.DamageTypesToResistance[DamageType];
			
			ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InTagsToDefs[ResistanceTag], EvaluateParams, TargetDeBuffResistance);

			TargetDeBuffResistance = FMath::Max<float>(TargetDeBuffResistance, 0.f);
			const auto EffectiveDeBuffChance = SourceDeBuffChance * ( 100 - TargetDeBuffResistance ) / 100.f;

			if (const auto IsDeBuff = FMath::RandRange(1, 100) < EffectiveDeBuffChance)
			{
				auto ContextHandle = GESpec.GetContext();
				UAuraAbilitySystemFuncLibrary::SetIsSuccessfulDeBuff(ContextHandle, IsDeBuff);
				UAuraAbilitySystemFuncLibrary::SetDamageType(ContextHandle, DamageType);

				const auto DeBuffDamage = GESpec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Damage, false, -1.f);
				const auto DeBuffDuration = GESpec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Duration, false, -1.f);
				const auto DeBuffFrequency = GESpec.GetSetByCallerMagnitude(GameplayTags.DeBuff_Frequency, false, -1.f);

				UAuraAbilitySystemFuncLibrary::SetDeBuffDamage(ContextHandle, DeBuffDamage);
				UAuraAbilitySystemFuncLibrary::SetDeBuffDuration(ContextHandle, DeBuffDuration);
				UAuraAbilitySystemFuncLibrary::SetDeBuffFrequency(ContextHandle, DeBuffFrequency);
			}
		}
	}
}

void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                        FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> Map_TagsToCaptureDef;
	
	const auto Tags = FAuraGameplayTags::Get();
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_Armor, DamageStatics().ArmorDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_ArmorPenetration, DamageStatics().ArmorPenetrationDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_BlockChance, DamageStatics().BlockChanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitChance, DamageStatics().CriticalHitChanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitResistance, DamageStatics().CriticalHitResistanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Secondary_CriticalHitDamage, DamageStatics().CriticalHitDamageDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Resistance_Fire, DamageStatics().FireResistanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Resistance_Lightning, DamageStatics().LightningResistanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Resistance_Arcane, DamageStatics().ArcaneResistanceDef);
	Map_TagsToCaptureDef.Add(Tags.Attributes_Resistance_Physical, DamageStatics().PhysicalResistanceDef);
	
	
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	if (!SourceAvatar || !TargetAvatar) return;

	int32 SourcePlayerLevel = 1;
	int32 TargetPlayerLevel = 1;
	if (SourceAvatar->Implements<UCombatInterface>())
	{
		SourcePlayerLevel = ICombatInterface::Execute_GetPlayerLevel(SourceAvatar);
	}
	if (TargetAvatar->Implements<UCombatInterface>())
	{
		TargetPlayerLevel = ICombatInterface::Execute_GetPlayerLevel(TargetAvatar);
	}

	const auto GESpec = ExecutionParams.GetOwningSpec();
	const auto SourceTag = GESpec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTag = GESpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	// Get Custom Effect Context => IsBlocked, IsCriticalHit for FloatingText Show Type
	auto EffectContextHandle = GESpec.GetContext();

	// DeBuff
	DetermineDeBuff(ExecutionParams, GESpec, EvaluateParams, Map_TagsToCaptureDef);
	
	// Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	
	for (const auto& Pair : FAuraGameplayTags::Get().DamageTypesToResistance)
	{
		const auto DamageTypeTag = Pair.Key;
		const auto ResistanceTag = Pair.Value;
		if (!(Map_TagsToCaptureDef.Contains(ResistanceTag)))
		{
			UE_LOG(Aura, Warning, TEXT("Map_TagsToCaptureDef does not contain ResistanceTag: %s"), *ResistanceTag.ToString());
		}
		const auto CaptureDef = Map_TagsToCaptureDef[ResistanceTag];

		float DamageTypeValue = GESpec.GetSetByCallerMagnitude(DamageTypeTag, false);
		float Resistance = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluateParams, Resistance);
		Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

		DamageTypeValue *= (100.f - Resistance) / 100.f;
		Damage += DamageTypeValue;
	}

	// Capture BlockChance On Target and Determinate if it is Blocked
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// If Blocked => Halve the Damage
	const auto bIsBlocked = FMath::FRandRange(1.f, 100.f) < TargetBlockChance;
	Damage = bIsBlocked ? Damage / 2.f : Damage;
	UAuraAbilitySystemFuncLibrary::SetIsBlockedHit(EffectContextHandle, bIsBlocked);

	// Capture Armor Penetration On Source and Armor On Target
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorDef, EvaluateParams, TargetArmor);
	TargetArmor = FMath::Max(0.f, TargetArmor);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().ArmorPenetrationDef, EvaluateParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(0.f, SourceArmorPenetration);

	// Get ArmorPenetration, EffectiveArmor Coefficient From CharacterClassInfo
	const auto CharacterClassInfo = UAuraAbilitySystemFuncLibrary::GetCharacterClassInfo(SourceAvatar);
	const auto ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName{"ArmorPenetration"}, FString{});
	const auto EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName{"EffectiveArmor"}, FString{});

	if (!ArmorPenetrationCurve || !EffectiveArmorCurve) return;
	const auto ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourcePlayerLevel);
	const auto EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetPlayerLevel);
	
	// ArmorPenetration ignores a percentage of the target's armor
	const auto EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	// Armor ignores a percentage of the IncomingDamage
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	// Capture CriticalHitChance On Source and CriticalHitResistance On Target
	float SourceCriticalHitChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitChanceDef, EvaluateParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(0.f, SourceCriticalHitChance);

	float TargetCriticalHitResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitResistanceDef, EvaluateParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(0.f, TargetCriticalHitResistance);

	float SourceCriticalHitDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().CriticalHitDamageDef, EvaluateParams, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(0.f, SourceCriticalHitDamage);

	// Get CriticalHitResistance From CharacterClassInfo
	const auto CriticalHitResistanceCurve = CharacterClassInfo->DamageCalculationCoefficient->FindCurve(FName{"CriticalHitResistance"}, FString{});
	if (!CriticalHitResistanceCurve) return;
	const auto CriticalHitResistanceCoefficient = CriticalHitResistanceCurve->Eval(TargetPlayerLevel);

	// Critical Hit Resistance reduces Critical Hit Chance by a certain percentage
	const auto EffectiveCriticalHitChance = SourceCriticalHitChance - TargetCriticalHitResistance * CriticalHitResistanceCoefficient;
	const auto bIsCriticalHit = FMath::FRandRange(1.f, 100.f) < EffectiveCriticalHitChance;
	Damage = bIsCriticalHit ? 2.f * Damage + SourceCriticalHitDamage : Damage;
	UAuraAbilitySystemFuncLibrary::SetIsCriticalHit(EffectContextHandle, bIsCriticalHit);

	// Set Last Modifier
	const FGameplayModifierEvaluatedData Modifier(
		UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	// Last Output
	OutExecutionOutput.AddOutputModifier(Modifier);
}
