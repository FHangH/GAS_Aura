// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

class UAuraAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTagDelegate, const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE(FAbilityGivenDelegate);
DECLARE_DELEGATE_OneParam(FForEachAbilityDelegate, const FGameplayAbilitySpec& /*Spec*/);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FAbilityStatusChangedDelegate, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const int32 /*AbilityLevel*/);
DECLARE_MULTICAST_DELEGATE_FourParams(FAbilityEquippedDelegate, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const FGameplayTag& /*SlotTag*/, const FGameplayTag& /*PrevSlot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FDeactivatePassiveAbilityDelegate, const FGameplayTag& /*AbilityTag*/);

UCLASS()
class GAS_AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* Property */
public:
	bool bStartupAbilitiesGiven {false};
	
	FEffectAssetTagDelegate OnEffectAssetTagDelegate;
	FAbilityGivenDelegate OnAbilityGivenDelegate;
	FAbilityStatusChangedDelegate OnAbilityStatusChangedDelegate;
	FAbilityEquippedDelegate OnAbilityEquippedDelegate;
	FDeactivatePassiveAbilityDelegate OnDeactivatePassiveAbilityDelegate;

	/* Function */
public:
	virtual void OnRep_ActivateAbilities() override;
	
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities);

	void AbilityInputPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FForEachAbilityDelegate& Delegate);
	bool IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const;

	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	static FGameplayTag GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& AbilityTag);
	FGameplayTag GetSlotFromAbilityTag(const FGameplayTag& AbilityTag);

	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& AbilityTag);
	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatuses(const int32 Level);

	bool GetDescriptionFromAbilityTag(const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription);

	// Slot
	bool SlotIsEmpty(const FGameplayTag& SlotTag);
	static bool AbilityHasSlot(const FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag);
	static bool AbilityHasAnySlot(const FGameplayAbilitySpec* Spec);
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& SlotTag);
	static void ClearSlot(FGameplayAbilitySpec* Spec);
	void ClearAbilitiesOfSlot(const FGameplayTag& SlotTag);
	static void AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& SlotTag);

	// RPC
public:
	UFUNCTION(Client, Reliable)
	void Client_OnEffectApplied(UAbilitySystemComponent* ASComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const;

	UFUNCTION(Server, Reliable)
	void Server_UpgradeAttribute(const FGameplayTag& AttributeTag);

	UFUNCTION(Client, Reliable)
	void Client_UpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel);

	UFUNCTION(Server, Reliable)
	void Server_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag);

	UFUNCTION(Client, Reliable)
	void Client_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot);
	
	UFUNCTION(Server, Reliable)
	void Server_SpendSpellPoint(const FGameplayTag& AbilityTag);
};
