// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "Untils/AuraGameplayTags.h"
#include "SpellMenuWidgetController.generated.h"

struct FAuraGameplayTags;
struct FGameplayTag;

struct FSelectedAbility
{
	FGameplayTag Ability {};
	FGameplayTag Status {};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FSpellGlobeSelectedSignature, const bool, IsSpendPointsButtonEnabled, const bool, IsEquipButtonEnabled, const FString, DescriptionString, const FString, NextLevelDescriptionString);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWaitForEquipSelectionSignature, const FGameplayTag&, AbilityType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpellGlobeReassignedSignature, const FGameplayTag&, AbilityTag);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
private:
	FSelectedAbility SelectedAbility { FAuraGameplayTags::Get().Ability_None, FAuraGameplayTags::Get().Ability_Status_Locked };

	int32 CurrentSpendPoints { 0 };
	
	bool bWaitingForEquipSelection { false };

	FGameplayTag SelectedSlot;
	
protected:
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Messages")
	FOnPlayerStatusChangedSignature OnSpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Status")
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Equip")
	FWaitForEquipSelectionSignature OnWaitForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Equip")
	FWaitForEquipSelectionSignature OnStopForEquipSelectionDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Reassign")
	FSpellGlobeReassignedSignature OnSpellGlobeReassignedDelegate;

	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;

	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void GlobeDeselect();
	
	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void SpendPointButtonPressed();

	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void EquipButtonPressed();

	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void SpellRowGlobePressed(const FGameplayTag& SlotTag, const FGameplayTag& AbilityType);

protected:
	void OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewLevel);
	void OnSpellPointsChanged(const int32 SpellPoints);
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusSlot, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot);

	static void ShouldEnableButton(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& IsShouldEnableSpellPointsButton, bool& IsShouldEnableEquipButton);
};
