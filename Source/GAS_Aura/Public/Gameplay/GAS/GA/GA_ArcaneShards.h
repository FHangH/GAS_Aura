// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "GA_ArcaneShards.generated.h"

UCLASS()
class GAS_AURA_API UGA_ArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Aura|Shard")
	int MaxNumShards { 11 };
	
	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
