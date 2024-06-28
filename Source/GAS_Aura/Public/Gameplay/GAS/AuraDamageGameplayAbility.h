// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FTaggedMontage;

UCLASS()
class GAS_AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category="Aura")
	static FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages);

protected:
	float GetDamageByDamageType(const float InLevel, const FGameplayTag& DamageType);
};
