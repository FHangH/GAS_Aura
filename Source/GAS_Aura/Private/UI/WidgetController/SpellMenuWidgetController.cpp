﻿// Copyright fangh.space


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Untils/AuraGameplayTags.h"

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

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		bWaitingForEquipSelection = false;
		const auto SelectedAbilityType = DataAsset_AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		OnStopForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
	}
	
	if (!GetAuraPS()) return;
	const auto GameplayTags = FAuraGameplayTags::Get();
	const auto AbilitySpec = GetAuraASC()->GetSpecFromAbilityTag(AbilityTag);

	const auto IsTagValid = AbilityTag.IsValid();
	const auto IsTagNone = AbilityTag.MatchesTag(GameplayTags.Ability_None);
	const auto IsSpecValid = AbilitySpec != nullptr;
	
	FGameplayTag AbilityStatus;
	if (!IsTagValid || IsTagNone || !IsSpecValid)
	{
		AbilityStatus = GameplayTags.Ability_Status_Locked;
	}
	else
	{
		AbilityStatus = GetAuraASC()->GetStatusTagFromSpec(*AbilitySpec);
	}
	SelectedAbility = { AbilityTag, AbilityStatus };

	bool IsEnableSpendPoint { false };
	bool IsEnableEquip { false };
	ShouldEnableButton(AbilityStatus, GetAuraPS()->GetSpellPoints(), IsEnableSpendPoint, IsEnableEquip);
	
	FString Description {};
	FString NextLevelDescription {};
	if (GetAuraASC())
	{
		GetAuraASC()->GetDescriptionFromAbilityTag(AbilityTag, Description, NextLevelDescription);
	}
	OnSpellGlobeSelectedDelegate.Broadcast(IsEnableSpendPoint, IsEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		bWaitingForEquipSelection = false;
		const auto SelectedAbilityType = DataAsset_AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		OnStopForEquipSelectionDelegate.Broadcast(SelectedAbilityType);
	}
	SelectedAbility = {FAuraGameplayTags::Get().Ability_None, FAuraGameplayTags::Get().Ability_Status_Locked};
	OnSpellGlobeSelectedDelegate.Broadcast(false, false, {}, {});
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	if (GetAuraASC())
	{
		GetAuraASC()->Server_SpendSpellPoint(SelectedAbility.Ability);	
	}
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const auto AbilityType = DataAsset_AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	bWaitingForEquipSelection = true;
	OnWaitForEquipSelectionDelegate.Broadcast(AbilityType);
}

void USpellMenuWidgetController::OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 NewLevel)
{
	if (SelectedAbility.Ability.MatchesTag(AbilityTag))
	{
		SelectedAbility.Status = StatusTag;

		bool IsEnableSpendPoint { false };
		bool IsEnableEquip { false };
		ShouldEnableButton(StatusTag, CurrentSpendPoints, IsEnableSpendPoint, IsEnableEquip);

		FString Description {};
		FString NextLevelDescription {};
		if (GetAuraASC())
		{
			GetAuraASC()->GetDescriptionFromAbilityTag(AbilityTag, Description, NextLevelDescription);
		}
		OnSpellGlobeSelectedDelegate.Broadcast(IsEnableSpendPoint, IsEnableEquip, Description, NextLevelDescription);
	}
	if (DataAsset_AbilityInfo)
	{
		auto Info = DataAsset_AbilityInfo->FindAbilityInfoForTag(AbilityTag);
		Info.StatusTag = StatusTag;
	
		OnAbilityInfoDelegate.Broadcast(Info);
	}
}

void USpellMenuWidgetController::OnSpellPointsChanged(const int32 SpellPoints)
{
	CurrentSpendPoints = SpellPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);

	bool IsEnableSpendPoint { false };
	bool IsEnableEquip { false };
	ShouldEnableButton(SelectedAbility.Status, CurrentSpendPoints, IsEnableSpendPoint, IsEnableEquip);

	FString Description {};
	FString NextLevelDescription {};
	if (GetAuraASC())
	{
		GetAuraASC()->GetDescriptionFromAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
	}
	OnSpellGlobeSelectedDelegate.Broadcast(IsEnableSpendPoint, IsEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::ShouldEnableButton(
	const FGameplayTag& AbilityStatus, const int32 SpellPoints, bool& IsShouldEnableSpellPointsButton, bool& IsShouldEnableEquipButton)
{
	const auto GameplayTags = FAuraGameplayTags::Get();
	
	IsShouldEnableSpellPointsButton = false;
	IsShouldEnableEquipButton = false;
	
	if (AbilityStatus.MatchesTag(GameplayTags.Ability_Status_Equipped))
	{
		if (SpellPoints > 0)
		{
			IsShouldEnableSpellPointsButton = true;
		}
		IsShouldEnableEquipButton = true;
	}
	if (AbilityStatus.MatchesTag(GameplayTags.Ability_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			IsShouldEnableSpellPointsButton = true;
		}
	}
	if (AbilityStatus.MatchesTag(GameplayTags.Ability_Status_Unlocked))
	{
		if (SpellPoints > 0)
		{
			IsShouldEnableSpellPointsButton = true;
		}
		IsShouldEnableEquipButton = true;
	}
}