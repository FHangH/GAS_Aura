﻿// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Async_WaitCooldownChange.generated.h"

class UAbilitySystemComponent;
struct FGameplayEffectSpec;
struct FActiveGameplayEffectHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownChangeSignature, float, TimeRemaining);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class GAS_AURA_API UAsync_WaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintAssignable, Category="Aura|Cooldown", DisplayName="CooldownStart")
	FCooldownChangeSignature CooldownStartDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|Cooldown", DisplayName="CooldownEnd")
	FCooldownChangeSignature CooldownEndDelegate;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AuraASC;

	UPROPERTY()
	FGameplayTag CooldownTag;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura|AsyncTask|Cooldown", DisplayName="WaitCooldownChange_Async", meta=(BlueprintInternalUseOnly=true))
	static UAsync_WaitCooldownChange* Async_WaitCooldownChange(UAbilitySystemComponent* ASC, const FGameplayTag InCooldownTag);

	UFUNCTION(BlueprintCallable, Category="Aura|AsyncTask|Cooldown")
	void EndTask();

	void OnActiveEffectAdded(UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpecApplied, FActiveGameplayEffectHandle ActiveGEHandle) const;
	void OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount) const;
};
