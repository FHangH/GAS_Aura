// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxMana.generated.h"

UCLASS()
class GAS_AURA_API UMMC_MaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	/* Property */
private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;

	/* Function */
public:
	UMMC_MaxMana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
