// Copyright fangh.space


#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/GAS/Data/DataAsset_LevelUpInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

void UOverlayMainWidgetController::BroadcastInitValues()
{
	if (GetAuraAS())
	{
		OnHealthChangedDelegate.Broadcast(AuraAS->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAS->GetMaxHealth());
		OnManaChangedDelegate.Broadcast(AuraAS->GetMana());
		OnMaxManaChangedDelegate.Broadcast(AuraAS->GetMaxMana());
	}
}

void UOverlayMainWidgetController::BindCallBackToDependencies()
{
	if (GetAuraPS())
	{
		AuraPlayerState->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
		AuraPlayerState->OnLevelChangedDelegate.AddUObject(this, &ThisClass::OnLevelChanged);
	}
	
	if (GetAuraAS() && ASComponent && GetAuraASC())
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetManaAttribute()).AddUObject(this, &ThisClass::OnManaChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAS->GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnMaxManaChanged);

		GetAuraASC()->OnAbilityEquippedDelegate.AddUObject(this, &ThisClass::OnAbilityEquipped);
		
		if (GetAuraASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetAuraASC()->OnAbilityGivenDelegate.AddUObject(this, &ThisClass::BroadcastAbilityInfo);
		}
		GetAuraASC()->OnEffectAssetTagDelegate.AddUObject(this, &ThisClass::OnEffectAssetTag);
	}
}

void UOverlayMainWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::OnManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::OnMaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChangedDelegate.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::OnEffectAssetTag(const FGameplayTagContainer& AssetTags) const
{
	const auto MessageTag = FGameplayTag::RequestGameplayTag(FName{"Message"});
	
	for (const auto& Tag : AssetTags)
	{
		if (!Tag.MatchesTag(MessageTag)) continue;
		if (const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag))
		{
			OnMessageWidgetRowDelegate.Broadcast(*Row);
		}
	}
}

void UOverlayMainWidgetController::OnXPChanged(const int32 NewXP)
{
	if (!GetAuraPS()) return;

	const auto LevelUpInfo = AuraPlayerState->DA_LevelUpInfo;
	if (!LevelUpInfo)
	{
		UE_LOG(Aura, Warning, TEXT("DataAsset_LevelUpInfo is nullptr, please check the DataAsset_LevelUpInfo in the AuraPlayerState"));
		return;
	}

	const auto Level = LevelUpInfo->FindLevelForXP(NewXP);
	const auto MaxLevel = LevelUpInfo->LevelUpInformation.Num();
	if (Level <= MaxLevel && Level > 0)
	{
		const auto LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const auto PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;
		
		const auto DeltaLevelUpRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const auto XPForThisLevel = NewXP - PreviousLevelUpRequirement;

		const auto XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelUpRequirement);
		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayMainWidgetController::OnLevelChanged(const int32 NewLevel, const bool IsLevelUp) const
{
	OnPlayerLevelChangedSignature.Broadcast(NewLevel, IsLevelUp);
}

void UOverlayMainWidgetController::OnAbilityEquipped(
	const FGameplayTag& AbilityTag, const FGameplayTag& StatusSlot, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot) const
{
	const auto GameplayTags = FAuraGameplayTags::Get();
	
	FAuraAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTags.Ability_Status_Unlocked;
	LastSlotInfo.InputTag = PreviousSlot;
	LastSlotInfo.AbilityTag = GameplayTags.Ability_None;
	OnAbilityInfoDelegate.Broadcast(LastSlotInfo);

	FAuraAbilityInfo Info = DataAsset_AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = StatusSlot;
	Info.InputTag = SlotTag;
	OnAbilityInfoDelegate.Broadcast(Info);
}
