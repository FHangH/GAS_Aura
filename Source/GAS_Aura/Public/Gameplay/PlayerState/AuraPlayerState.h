// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class GAS_AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AS;

	/* Function */
public:
	AAuraPlayerState();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AS; }
};
