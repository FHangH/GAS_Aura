// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagDelegate, const FGameplayTagContainer& /*AssetTags*/)
DECLARE_MULTICAST_DELEGATE(FAbilityGivenDelegate)
DECLARE_DELEGATE_OneParam(FForEachAbilityDelegate, const FGameplayAbilitySpec& /*Spec*/)

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Property */
public:
	FEffectAssetTagDelegate EffectAssetTagDelegate;
	FAbilityGivenDelegate AbilityGivenDelegate;

	bool bStartupAbilitiesGiven {false};

	/* Function */
public:
	virtual void OnRep_ActivateAbilities() override;
	
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilityDelegate& Delegate);

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
protected:
	UFUNCTION(Client, Reliable)
	void Client_OnEffectApplied(UAbilitySystemComponent* ASComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const;

	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag);
};
