// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GA_AuraBeamSpell.h"
#include "GA_AuraElectrocute.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraElectrocute : public UGA_AuraBeamSpell
{
	GENERATED_BODY()

	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
