// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

UCLASS()
class GAS_AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FGameplayTag StartUpInputTag;
};
