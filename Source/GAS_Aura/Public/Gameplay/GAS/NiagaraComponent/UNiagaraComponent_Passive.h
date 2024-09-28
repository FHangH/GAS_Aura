// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "UNiagaraComponent_Passive.generated.h"


UCLASS(ClassGroup=(Aura), meta=(BlueprintSpawnableComponent))
class GAS_AURA_API UUNiagaraComponent_Passive : public UNiagaraComponent
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FGameplayTag PassiveSpell_Tag;

	/* Function */
public:
	UUNiagaraComponent_Passive();

protected:
	virtual void BeginPlay() override;

	void OnPassiveActive(const FGameplayTag& AbilityTag, const bool bActivate);
};
