// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "GA_AuraFireBlast.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|FireBlast")
	int32 NumFireBalls { 12 };
	
	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
