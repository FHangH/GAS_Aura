// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
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
	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAttributeSet> AS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AsClass")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesClass;

	// Abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GAClass")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	// SocketNames
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName WeaponTipSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName LeftHandSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName RightHandSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName TailSocketName {};

	// Montages
	UPROPERTY(EditAnywhere, Category="Aura|Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Montage|Attack")
	TArray<FTaggedMontage> AttackMontages;

	// Dynamic Materials
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Material")
	TObjectPtr<UMaterialInstance> BodyDissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Material")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// Niagara System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Niagra")	
	TObjectPtr<UNiagaraSystem> BloodEffect;

	// Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Sound")	
	TObjectPtr<USoundBase> DeathSound;
	
	// Status
	bool bIsDead { false };
	
	/* Function */
public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	
	// Combat Interface
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;

public:
	UAttributeSet* GetAttributeSet() const { return AS; }
	
	UFUNCTION(NetMulticast, Reliable, Category="Aura")
	void Multicast_HandleDeath();
	
	void Dissolve();
	UFUNCTION(BlueprintNativeEvent, Category="Aura|Material")
	void StartBodyDissolveTimeLine(UMaterialInstanceDynamic* DynamicMatIns);
	UFUNCTION(BlueprintNativeEvent, Category="Aura|Material")
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMatIns);
};