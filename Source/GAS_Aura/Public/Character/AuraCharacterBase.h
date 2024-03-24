// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AuraCharacterBase.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class GAS_AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, Category="Aura")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AS;

	/* Function */
public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AS; }

protected:
	virtual void BeginPlay() override;
};