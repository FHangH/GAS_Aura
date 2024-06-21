// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTag;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSpellGlobeSelectedSignature, const bool, IsSpendPointsButtonEnabled, const bool, IsEquipButtonEnabled);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
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
	void OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const;
	void OnSpellPointsChanged(const int32 SpellPoints) const;

	static void ShouldEnableButton(const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& IsShouldEnableSpellPointsButton, bool& IsShouldEnableEquipButton);
};
