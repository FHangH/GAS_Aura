// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class GAS_AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, Category="Aura|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UAttributeSet> AS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GAClass", meta=(AllowPrivateAccess=true))
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName WeaponTipSocketName {};
	
	/* Function */
public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AS; }

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual FVector GetCombatSocketLocation() override;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;
};