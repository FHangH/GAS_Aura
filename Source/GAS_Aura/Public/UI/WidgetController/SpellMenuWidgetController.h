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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, const bool, IsSpendPointsButtonEnabled, const bool, IsEquipButtonEnabled);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
private:
	FSelectedAbility SelectedAbility { FAuraGameplayTags::Get().Ability_None, FAuraGameplayTags::Get().Ability_Status_Locked };
	int32 CurrentSpendPoints { 0 };
	
protected:
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Messages")
	FOnPlayerStatusChangedSignature OnSpellPointsChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Status")
	FSpellGlobeSelectedSignature OnSpellGlobeSelectedDelegate;

	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;

	UFUNCTION(BlueprintCallable, Category="Aura|WidgetController")
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);

protected:
	void OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
	void OnSpellPointsChanged(const int32 SpellPoints);

	static void ShouldEnableButton(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& IsShouldEnableSpellPointsButton, bool& IsShouldEnableEquipButton);
};
