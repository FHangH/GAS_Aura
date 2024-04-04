// Copyright fangh.space


#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;
	bReplicates = true;

	ASComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASComponent->SetIsReplicated(true);
	ASComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AS = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return ASComponent;
}

void AAuraPlayerState::OnRep_Level(const int32 OldLevel) const
{
	
}