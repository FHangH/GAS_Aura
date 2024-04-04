// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class GAS_AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesClass;
	
	/* Function */
public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AS; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	void InitializeDefaultAttributes() const;
};