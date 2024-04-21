// Copyright fangh.space


#include "Gameplay/GAS/ExecCalc/EC_Damage.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
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
}

void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	const auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	if (!SourceAvatar || !TargetAvatar) return;

	const auto SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	const auto TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);
	if (!SourceCombatInterface || !TargetCombatInterface) return;

	const auto GESpec = ExecutionParams.GetOwningSpec();

	const auto SourceTag = GESpec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTag = GESpec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParams;
	EvaluateParams.SourceTags = SourceTag;
	EvaluateParams.TargetTags = TargetTag;

	// Get Damage Set by Caller Magnitude
	float Damage = GESpec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);

	// Capture BlockChance On Target and Determinate if it is Blocked
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().BlockChanceDef, EvaluateParams, TargetBlockChance);
	TargetBlockChance = FMath::Max(0.f, TargetBlockChance);

	// If Blocked => Halve the Damage
	const auto bIsBlocked = FMath::FRandRange(1.f, 100.f) < TargetBlockChance;
	Damage = bIsBlocked ? Damage / 2.f : Damage;

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
	const auto ArmorPenetrationCoefficient = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	const auto EffectiveArmorCoefficient = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	
	// ArmorPenetration ignores a percentage of the target's armor
	const auto EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficient) / 100.f;
	// Armor ignores a percentage of the IncomingDamage
	Damage *= (100.f - EffectiveArmor * EffectiveArmorCoefficient) / 100.f;

	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Green, FString::Printf(TEXT("Damage: %f"), Damage));
	
	const FGameplayModifierEvaluatedData Modifier(
		UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(Modifier);
}
