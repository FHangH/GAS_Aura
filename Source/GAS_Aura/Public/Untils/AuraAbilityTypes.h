// Copyright fangh.space

#pragma once

#include "GameplayEffectTypes.h"
#include "AuraAbilityTypes.generated.h"

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