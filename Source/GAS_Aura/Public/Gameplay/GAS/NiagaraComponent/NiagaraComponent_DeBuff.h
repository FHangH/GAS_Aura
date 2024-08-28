// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraComponent.h"
#include "NiagaraComponent_DeBuff.generated.h"

UCLASS()
class GAS_AURA_API UNiagaraComponent_DeBuff : public UNiagaraComponent
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FGameplayTag DeBuff_Tag;

	/* Function */
public:
	UNiagaraComponent_DeBuff();

protected:
	virtual void BeginPlay() override;

	auto OnDeBuffTagChanged(const FGameplayTag CallBackTag, int32 NewCount) -> void;

	UFUNCTION()
	void OnOwnerDeath(AActor* DeadActor);
};
