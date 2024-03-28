// Copyright fangh.space


#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "Gameplay/GAS/AuraAttributeSet.h"

void UOverlayMainWidgetController::BroadcastInitValues()
{
	Super::BroadcastInitValues();

	if (const auto AuraAs = Cast<UAuraAttributeSet>(AS))
	{
		OnHealthChanged.Broadcast(AuraAs->GetHealth());
		OnMaxHealthChanged.Broadcast(AuraAs->GetMaxHealth());
		OnManaChanged.Broadcast(AuraAs->GetMana());
		OnMaxManaChanged.Broadcast(AuraAs->GetMaxMana());
	}
}

void UOverlayMainWidgetController::BindCallBackToDependencies()
{
	Super::BindCallBackToDependencies();

	const auto AuraAs = Cast<UAuraAttributeSet>(AS);
	if (AuraAs && ASComponent)
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetManaAttribute()).AddUObject(this, &ThisClass::ManaChanged);
		ASComponent->GetGameplayAttributeValueChangeDelegate(
			AuraAs->GetMaxManaAttribute()).AddUObject(this, &ThisClass::MaxManaChanged);
	}
}

void UOverlayMainWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::ManaChanged(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayMainWidgetController::MaxManaChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
