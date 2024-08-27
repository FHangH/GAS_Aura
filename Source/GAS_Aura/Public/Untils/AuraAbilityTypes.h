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
	bool bIsBlockedHit { false };
	
	UPROPERTY()
	bool bIsCriticalHit { false };

	UPROPERTY()
	bool bIsSuccessfulDeBuff { false };

	UPROPERTY()
	float DeBuffDamage { 0.f };

	UPROPERTY()
	float DeBuffDuration { 0.f };

	UPROPERTY()
	float DeBuffFrequency { 0.f };

	TSharedPtr<FGameplayTag> DamageType;
	
	/* Function */
public:
	virtual UScriptStruct* GetScriptStruct() const override;
	virtual bool NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess) override;
	virtual FGameplayEffectContext* Duplicate() const override;

	// Get
	FORCEINLINE bool IsCriticalHit() const { return bIsCriticalHit; }
	FORCEINLINE bool IsBlockedHit() const { return bIsBlockedHit; }
	FORCEINLINE bool IsSuccessfulDeBuff() const { return bIsSuccessfulDeBuff; }
	FORCEINLINE float GetDeBuffDamage() const { return DeBuffDamage; }
	FORCEINLINE float GetDeBuffDuration() const { return DeBuffDuration; }
	FORCEINLINE float GetDeBuffFrequency() const { return DeBuffFrequency; }
	FORCEINLINE TSharedPtr<FGameplayTag> GetDamageType() const { return DamageType; }

	// Set
	FORCEINLINE void SetIsCriticalHit(const bool bNewCriticalHit) { bIsCriticalHit = bNewCriticalHit; }
	FORCEINLINE void SetIsBlockedHit(const bool bNewBlockedHit) { bIsBlockedHit = bNewBlockedHit; }
	FORCEINLINE void SetIsSuccessfulDeBuff(const bool bNewSuccessfulDeBuff) { bIsSuccessfulDeBuff = bNewSuccessfulDeBuff; }
	FORCEINLINE void SetDeBuffDamage(const float InDeBuffDamage) { DeBuffDamage = InDeBuffDamage; }
	FORCEINLINE void SetDeBuffDuration(const float InDeBuffDuration) { DeBuffDuration = InDeBuffDuration; }
	FORCEINLINE void SetDeBuffFrequency(const float InDeBuffFrequency) { DeBuffFrequency = InDeBuffFrequency; }
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