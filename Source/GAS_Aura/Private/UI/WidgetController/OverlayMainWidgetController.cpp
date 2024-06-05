// Copyright fangh.space


#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/GAS/Data/DataAsset_LevelUpInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Untils/AuraLog.h"

void UOverlayMainWidgetController::BroadcastInitValues()
{
	Super::BroadcastInitValues();

	if (const auto AuraAs = Cast<UAuraAttributeSet>(AS))
	{
		OnHealthChangedDelegate.Broadcast(AuraAs->GetHealth());
		OnMaxHealthChangedDelegate.Broadcast(AuraAs->GetMaxHealth());
		OnManaChangedDelegate.Broadcast(AuraAs->GetMana());
		OnMaxManaChangedDelegate.Broadcast(AuraAs->GetMaxMana());
	}
}

void UOverlayMainWidgetController::BindCallBackToDependencies()
{
	Super::BindCallBackToDependencies();

	if (const auto AuraPS = Cast<AAuraPlayerState>(PlayerState))
	{
		AuraPS->OnXPChangedDelegate.AddUObject(this, &ThisClass::OnXPChanged);
	}
	
	const auto AuraAs = Cast<UAuraAttributeSet>(AS);
	
	if (AuraAs && ASComponent)
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::OnMaxHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetManaAttribute()).AddUObject(this, &ThisClass::OnManaChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetMaxManaAttribute()).AddUObject(this, &ThisClass::OnMaxManaChanged);

		if (auto AuraASC = Cast<UAuraAbilitySystemComponent>(ASComponent))
		{
			if (AuraASC->bStartupAbilitiesGiven)
			{
				OnInitializeStartupAbilities(AuraASC);
			}
			else
			{
				AuraASC->AbilityGivenDelegate.AddUObject(this, &ThisClass::OnInitializeStartupAbilities);
			}
			AuraASC->EffectAssetTagDelegate.AddUObject(this, &ThisClass::OnEffectAssetTag);
		}
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

void UOverlayMainWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC) const
{
	if (!AuraASC || !AuraASC->bStartupAbilitiesGiven) return;

	FForEachAbilityDelegate ForEachAbilityDelegate;
	ForEachAbilityDelegate.BindLambda([this, AuraASC](const FGameplayAbilitySpec& AbilitySpec)
	{
		auto DA_AbilityInfo = DataAsset_AbilityInfo->FindAbilityInfoForTag(AuraASC->GetAbilityTagFromSpec(AbilitySpec));
		DA_AbilityInfo.InputTag = AuraASC->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(DA_AbilityInfo);
	});
	
	AuraASC->ForEachAbility(ForEachAbilityDelegate);
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

void UOverlayMainWidgetController::OnXPChanged(const int32 NewXP) const
{
	const auto AuraPS = Cast<AAuraPlayerState>(PlayerState);
	if (!AuraPS) return;

	const auto LevelUpInfo = AuraPS->DA_LevelUpInfo;
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
