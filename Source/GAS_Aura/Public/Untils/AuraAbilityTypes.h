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

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	bool bIsRadialDamage { false };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float RadialDamageInnerRadius { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	float RadialDamageOuterRadius { 0.f };

	UPROPERTY(BlueprintReadWrite, Category="Aura|DamageEffectParam")
	FVector RadialDamageOrigin { FVector::ZeroVector };
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

	UPROPERTY()
	bool bIsRadialDamage { false };

	UPROPERTY()
	float RadialDamageInnerRadius { 0.f };

	UPROPERTY()
	float RadialDamageOuterRadius { 0.f };

	UPROPERTY()
	FVector RadialDamageOrigin { FVector::ZeroVector };
	
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
	FORCEINLINE bool IsRadialDamage() const { return bIsRadialDamage; }
	FORCEINLINE float GetRadialDamageInnerRadius() const { return RadialDamageInnerRadius; }
	FORCEINLINE float GetRadialDamageOuterRadius() const { return RadialDamageOuterRadius; }
	FORCEINLINE FVector GetRadialDamageOrigin() const { return RadialDamageOrigin; }

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
	FORCEINLINE void SetIsRadialDamage(const bool bNewIsRadialDamage) { bIsRadialDamage = bNewIsRadialDamage; }
	FORCEINLINE void SetRadialDamageInnerRadius(const float InRadialDamageInnerRadius) { RadialDamageInnerRadius = InRadialDamageInnerRadius; }
	FORCEINLINE void SetRadialDamageOuterRadius(const float InRadialDamageOuterRadius) { RadialDamageOuterRadius = InRadialDamageOuterRadius; }
	FORCEINLINE void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin) { RadialDamageOrigin = InRadialDamageOrigin; }
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