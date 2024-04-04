// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_MaxHealth.generated.h"

UCLASS()
class GAS_AURA_API UMMC_MaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	/* Property */
private:
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;

	/* Function */
public:
	UMMC_MaxHealth();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
