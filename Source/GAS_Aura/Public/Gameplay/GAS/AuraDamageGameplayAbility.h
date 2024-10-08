// Copyright fangh.space

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
	float DeathImpulseMagnitude { 18000.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Impulse")
	float KnockBackForceMagnitude { 5000.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Impulse")
	float KnockBackChance { 10.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|RadialDamage")
	bool bIsRadialDamage { false };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|RadialDamage")
	float RadialDamageInnerRadius { 0.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|RadialDamage")
	float RadialDamageOuterRadius { 0.f };

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|RadialDamage")
	FVector RadialDamageOrigin { FVector::ZeroVector };
	
	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void CauseDamage(AActor* TargetActor) const;

	UFUNCTION(BlueprintPure, Category="Aura")
	static FTaggedMontage GetRandomTaggedMontage(const TArray<FTaggedMontage>& TaggedMontages);

	UFUNCTION(BlueprintPure, Category="Aura")
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	UFUNCTION(BlueprintPure, Category="Aura")
	float GetDamageAtLevel() const;
};
