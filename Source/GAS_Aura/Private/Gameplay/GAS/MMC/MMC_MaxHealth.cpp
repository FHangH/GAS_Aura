// Copyright fangh.space


#include "Gameplay/GAS/MMC/MMC_MaxHealth.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDefinition.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const auto SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const auto TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = SourceTag;
	EvalParams.TargetTags = TargetTag;

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvalParams, Vigor);
	Vigor = FMath::Max<float>(Vigor, 0.f);

	if (const auto CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject()))
	{
		const auto PlayerLevel = CombatInterface->GetPlayerLevel();

		return 80.f + 2.5f * Vigor + 10.f * PlayerLevel;
	}
	return 0.f;
}
