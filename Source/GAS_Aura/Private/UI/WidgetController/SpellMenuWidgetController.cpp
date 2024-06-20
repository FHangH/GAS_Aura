// Copyright fangh.space


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"

void USpellMenuWidgetController::BindCallBackToDependencies()
{
	if (!GetAuraASC()) return;
	GetAuraASC()->OnAbilityStatusChangedDelegate.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
	{
		if (!DataAsset_AbilityInfo) return;

		auto Info = DataAsset_AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		Info.StatusTag = StatusTag;
		OnAbilityInfoDelegate.Broadcast(Info);
	});
}

void USpellMenuWidgetController::BroadcastInitValues()
{
	BroadcastAbilityInfo();
}
