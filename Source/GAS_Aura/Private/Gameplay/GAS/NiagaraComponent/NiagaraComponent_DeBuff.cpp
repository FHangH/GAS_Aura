// Copyright fangh.space


#include "Gameplay/GAS/NiagaraComponent/NiagaraComponent_DeBuff.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Interaction/CombatInterface.h"

UNiagaraComponent_DeBuff::UNiagaraComponent_DeBuff()
{
	bAutoActivate = false;
}

void UNiagaraComponent_DeBuff::BeginPlay()
{
	Super::BeginPlay();

	const auto CombatInterface = Cast<ICombatInterface>(GetOwner());
	
	if (const auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner()))
	{
		ASC->RegisterGameplayTagEvent(DeBuff_Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnDeBuffTagChanged);
	}
	else if (CombatInterface)
	{
		CombatInterface->GetOnAsComponentRegisteredDelegate().AddLambda([this](UAbilitySystemComponent* NewASC)
		{
			NewASC->RegisterGameplayTagEvent(DeBuff_Tag, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::OnDeBuffTagChanged);
		});
	}

	if (CombatInterface)
	{
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::OnOwnerDeath);
	}
}

void UNiagaraComponent_DeBuff::OnDeBuffTagChanged(const FGameplayTag CallBackTag, const int32 NewCount)
{
	NewCount > 0 ? Activate() : Deactivate();
}

void UNiagaraComponent_DeBuff::OnOwnerDeath(AActor* DeadActor)
{
	Deactivate();
}