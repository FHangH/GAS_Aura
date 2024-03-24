// Copyright fangh.space


#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	ASComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASComponent->SetIsReplicated(true);
	ASComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AS = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return ASComponent;
}
