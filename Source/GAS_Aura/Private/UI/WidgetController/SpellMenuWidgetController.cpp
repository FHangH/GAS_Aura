// Copyright fangh.space


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"

void USpellMenuWidgetController::BindCallBackToDependencies()
{
	if (GetAuraASC())
	{
		GetAuraASC()->OnAbilityStatusChangedDelegate.AddUObject(this, &ThisClass::OnAbilityStatusChanged);
	}
	if (GetAuraPS())
	{
		GetAuraPS()->OnSpellPointChangedDelegate.AddUObject(this, &ThisClass::OnSpellPointsChanged);
	}
}

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();

	if (GetAuraPS())
	{
		OnSpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());
	}
}

void USpellMenuWidgetController::OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const
{
	if (!DataAsset_AbilityInfo) return;
	
	auto Info = DataAsset_AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusTag;
	OnAbilityInfoDelegate.Broadcast(Info);
}

void USpellMenuWidgetController::OnSpellPointsChanged(const int32 SpellPoints) const
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
