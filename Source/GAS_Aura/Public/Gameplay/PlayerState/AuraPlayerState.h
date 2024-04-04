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
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AS;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level, Category="Aura")
	int32 Level {1};
	
	/* Function */
public:
	AAuraPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Inline Get
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AS; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }

	UFUNCTION()
	void OnRep_Level(const int32 OldLevel) const;
};
