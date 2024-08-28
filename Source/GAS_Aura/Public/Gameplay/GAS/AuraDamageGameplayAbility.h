﻿// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
struct FTaggedMontage;

UCLASS()
class GAS_AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE")
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	FGameplayTag DamageType;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Damage")
	FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|DeBuff")
	float DeBuffChance { 20.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|DeBuff")
	float DeBuffDamage { 5.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|DeBuff")
	float DeBuffFrequency { 1.f };
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|DeBuff")
	float DeBuffDuration { 5.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Impulse")
	float DeathImpulseMagnitude { 60.f };
	
	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void CauseDamage(AActor* TargetActor) const;

	UFUNCTION(BlueprintPure, Category="Aura")
	static FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages);

	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(TObjectPtr<AActor> TargetActor = nullptr) const;
};
