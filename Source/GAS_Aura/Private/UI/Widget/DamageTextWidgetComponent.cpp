// Copyright fangh.space


#include "UI/Widget/DamageTextWidgetComponent.h"

UDamageTextWidgetComponent::UDamageTextWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDamageTextWidgetComponent::SetDamageText_Implementation(const float Damage, const bool IsBlockedHit, const bool IsCriticalHit)
{
	
}