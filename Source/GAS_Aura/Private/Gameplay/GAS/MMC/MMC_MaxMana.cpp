// Copyright fangh.space


#include "Gameplay/GAS/MMC/MMC_MaxMana.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDefinition.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const auto SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTag;
	EvalParams.TargetTags = TargetTag;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvalParams, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	if (const auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		const auto PlayerLevel = CombatInterface->GetPlayerLevel();

		return 50.f + 2.5f * Intelligence + 15.f * PlayerLevel;
	}
	return 0.f;
}
