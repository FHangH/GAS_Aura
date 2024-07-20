// Copyright fangh.space

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_USTRUCT_BODY()

	FDamageEffectParams(){}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject { nullptr };

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass { nullptr };

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceASComponent { nullptr };

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetASComponent { nullptr };

	UPROPERTY()
	float BaseDamage { 0.f };

	UPROPERTY()
	float AbilityLevel { 1.f };

	UPROPERTY()
	FGameplayTag DamageType { FGameplayTag{} };

	UPROPERTY()
	float DeBuffChance { 0.f };

	UPROPERTY()
	float DeBuffDamage { 0.f };

	UPROPERTY()
	float DeBuffDuration { 0.f };

	UPROPERTY()
	float DeBuffFrequency { 0.f };
};

USTRUCT(BlueprintType)
struct FAuraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_USTRUCT_BODY()

	/* Property */
protected:
	UPROPERTY()
	bool bIsBlockedHit {false};
	
	UPROPERTY()
	bool bIsCriticalHit {false};
	
	/* Function */
public:
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	virtual FGameplayEffectContext* Duplicate() const override;

	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE void SetIsCriticalHit(const bool bNewCriticalHit) { bIsCriticalHit = bNewCriticalHit; }
	FORCEINLINE void SetIsBlockedHit(const bool bNewBlockedHit) { bIsBlockedHit = bNewBlockedHit; }
};

template<>
struct TStructOpsTypeTraits<FAuraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FAuraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true,
	};
};