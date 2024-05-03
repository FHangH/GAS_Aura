// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

UCLASS()
class GAS_AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void CauseDamage(AActor* TargetActor);
};
