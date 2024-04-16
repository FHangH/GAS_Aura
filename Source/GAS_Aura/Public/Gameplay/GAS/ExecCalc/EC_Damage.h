// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "EC_Damage.generated.h"

UCLASS()
class GAS_AURA_API UEC_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	/* Function */
public:
	UEC_Damage();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
