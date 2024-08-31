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

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	TObjectPtr<UObject> WorldContextObject { nullptr };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass { nullptr };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	TObjectPtr<UAbilitySystemComponent> SourceASComponent { nullptr };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	TObjectPtr<UAbilitySystemComponent> TargetASComponent { nullptr };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float BaseDamage { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float AbilityLevel { 1.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	FGameplayTag DamageType { FGameplayTag{} };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float DeBuffChance { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float DeBuffDamage { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float DeBuffDuration { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float DeBuffFrequency { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float DeathImpulseMagnitude { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	FVector DeathImpulse { FVector::ZeroVector };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float KnockBackForceMagnitude { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float KnockBackChance { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	FVector KnockBackForce { FVector::ZeroVector };
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

	UPROPERTY()
	FVector DeathImpulse { FVector::ZeroVector };

	UPROPERTY()
	FVector KnockBackForce { FVector::ZeroVector };
	
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
	FORCEINLINE FVector GetDeathImpulse() const { return DeathImpulse; }
	FORCEINLINE FVector GetKnockBackForce() const { return KnockBackForce; }

	// Set
	FORCEINLINE void SetIsCriticalHit(const bool bNewCriticalHit) { bIsCriticalHit = bNewCriticalHit; }
	FORCEINLINE void SetIsBlockedHit(const bool bNewBlockedHit) { bIsBlockedHit = bNewBlockedHit; }
	FORCEINLINE void SetIsSuccessfulDeBuff(const bool bNewSuccessfulDeBuff) { bIsSuccessfulDeBuff = bNewSuccessfulDeBuff; }
	FORCEINLINE void SetDeBuffDamage(const float InDeBuffDamage) { DeBuffDamage = InDeBuffDamage; }
	FORCEINLINE void SetDeBuffDuration(const float InDeBuffDuration) { DeBuffDuration = InDeBuffDuration; }
	FORCEINLINE void SetDeBuffFrequency(const float InDeBuffFrequency) { DeBuffFrequency = InDeBuffFrequency; }
	FORCEINLINE void SetDamageType(const TSharedPtr<FGameplayTag>& InDamageType) { DamageType = InDamageType; }
	FORCEINLINE void SetDeathImpulse(const FVector& InDeathImpulse) { DeathImpulse = InDeathImpulse; }
	FORCEINLINE void SetKnockBackForce(const FVector& InKnockBackFore) { KnockBackForce = InKnockBackFore; }
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