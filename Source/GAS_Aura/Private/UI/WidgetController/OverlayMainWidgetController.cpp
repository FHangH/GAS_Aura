// Copyright fangh.space


#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"

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
