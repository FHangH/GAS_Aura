// Copyright fangh.space


#include "Gameplay/GAS/ExecCalc/EC_Damage.h"
#include "AbilitySystemComponent.h"

UEC_Damage::UEC_Damage()
{
}

void UEC_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const auto TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	const auto SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	const auto TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	const auto GESpec = ExecutionParams.GetOwningSpec();
}
