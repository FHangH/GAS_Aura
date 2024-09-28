// Copyright fangh.space


#include "Gameplay/GAS/NiagaraComponent/UNiagaraComponent_Passive.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UUNiagaraComponent_Passive::UUNiagaraComponent_Passive()
{
	bAutoActivate = false;
}

void UUNiagaraComponent_Passive::BeginPlay()
{
	Super::BeginPlay();

	if (const auto AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		AuraASC->OnActivePassiveEffectDelegate.AddUObject(this, &ThisClass::OnPassiveActive);
	}
	else if (const auto CombatInterface = Cast<ICombatInterface>(GetOwner()); CombatInterface && AuraASC)
	{
		CombatInterface->GetOnAsComponentRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* InASC)
		{
			if (const auto AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
			{
				AuraASC->OnActivePassiveEffectDelegate.AddUObject(this, &ThisClass::OnPassiveActive);
			}
		});
	}
}

void UUNiagaraComponent_Passive::OnPassiveActive(const FGameplayTag& AbilityTag, const bool bActivate)
{
	if (!AbilityTag.MatchesTagExact(PassiveSpell_Tag)) return;
	if (bActivate && !IsActive())
	{
		Activate();
	}
	else
	{
		Deactivate();
	}
}
