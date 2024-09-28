// Copyright fangh.space

/**
 * This defines a set of helper functions for accessing and initializing attributes, to avoid having to manually write these functions.
 * It would Creates the following functions, for attribute Health
 *
 *	static FGameplayAttribute UMyHealthSet::GetHealthAttribute();
 *	FORCEINLINE float UMyHealthSet::GetHealth() const;
 *	FORCEINLINE void UMyHealthSet::SetHealth(float NewVal);
 *	FORCEINLINE void UMyHealthSet::InitHealth(float NewVal);
 *
 * To use this in your game you can define something like this, and then add game-specific functions as necessary:
 * 
 *	#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 *	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
 * 
 *	ATTRIBUTE_ACCESSORS(UMyHealthSet, Health)
 */

/**
 *	This is a helper macro that can be used in RepNotify functions to handle attributes that will be predictively modified by clients.
 *	
 *	void UMyHealthSet::OnRep_Health(const FGameplayAttributeData& OldValue)
 *	{
 *		GAMEPLAYATTRIBUTE_REPNOTIFY(UMyHealthSet, Health, OldValue);
 *	}
 */

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AuraAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

USTRUCT(BlueprintType)
struct FEffectProperties
{
	GENERATED_USTRUCT_BODY()

	FEffectProperties() {}

	/*EffectContextHandle, ASC, Avatar, Controller, Character*/
	FGameplayEffectContextHandle EffectContextHandle;
	
	// Source
	UPROPERTY()
	UAbilitySystemComponent* SourceASC { nullptr };
	UPROPERTY()
	AActor* SourceAvatarActor { nullptr };
	UPROPERTY()
	AController* SourceController { nullptr };
	UPROPERTY()
	ACharacter* SourceCharacter { nullptr };

	// Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC { nullptr };
	UPROPERTY()
	AActor* TargetAvatarActor { nullptr };
	UPROPERTY()
	AController* TargetController { nullptr };
	UPROPERTY()
	ACharacter* TargetCharacter { nullptr };
};

UCLASS()
class GAS_AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* Property */
public:
	// Primary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Strength, Category="Aura|AS|Primary")
	FGameplayAttributeData Strength {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Strength)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Intelligence, Category="Aura|AS|Primary")
	FGameplayAttributeData Intelligence {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Intelligence)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Resilience, Category="Aura|AS|Primary")
	FGameplayAttributeData Resilience {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Resilience)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Vigor, Category="Aura|AS|Primary")
	FGameplayAttributeData Vigor {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Vigor)

	// Secondary Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Armor, Category="Aura|AS|Secondary")
	FGameplayAttributeData Armor {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Armor)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArmorPenetration, Category="Aura|AS|Secondary")
	FGameplayAttributeData ArmorPenetration {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArmorPenetration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_BlockChance, Category="Aura|AS|Secondary")
	FGameplayAttributeData BlockChance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, BlockChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitChance, Category="Aura|AS|Secondary")
	FGameplayAttributeData CriticalHitChance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitChance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitDamage, Category="Aura|AS|Secondary")
	FGameplayAttributeData CriticalHitDamage {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitDamage)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_CriticalHitResistance, Category="Aura|AS|Secondary")
	FGameplayAttributeData CriticalHitResistance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, CriticalHitResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_HealthRegeneration, Category="Aura|AS|Secondary")
	FGameplayAttributeData HealthRegeneration {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, HealthRegeneration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ManaRegeneration, Category="Aura|AS|Secondary")
	FGameplayAttributeData ManaRegeneration {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ManaRegeneration)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Aura|AS|Secondary")
	FGameplayAttributeData MaxHealth {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Aura|AS|Secondary")
	FGameplayAttributeData MaxMana {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

	// Resistance Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_FireResistance, Category="Aura|AS|Resistance")
	FGameplayAttributeData FireResistance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, FireResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_LightningResistance, Category="Aura|AS|Resistance")
	FGameplayAttributeData LightningResistance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, LightningResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_ArcaneResistance, Category="Aura|AS|Resistance")
	FGameplayAttributeData ArcaneResistance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, ArcaneResistance)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_PhysicalResistance, Category="Aura|AS|Resistance")
	FGameplayAttributeData PhysicalResistance {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, PhysicalResistance)
	
	// Vital Attributes
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Aura|AS|Vital")
	FGameplayAttributeData Health {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Aura|AS|Vital")
	FGameplayAttributeData Mana {};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	// Meta Attributes => Only Server, No Replicate, Temporary Attribute Used To CalculateDamage
	UPROPERTY(BlueprintReadOnly, Category="Aura|AS|Meta")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingDamage)

	UPROPERTY(BlueprintReadOnly, Category="Aura|AS|Meta")
	FGameplayAttributeData IncomingXP;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, IncomingXP)
	
	// EffectContextHandle, ASC, Avatar, Controller, Character
	UPROPERTY(BlueprintReadOnly, Category="Aura|AS")
	FEffectProperties EffectProperties_Old {};

	// Use GameplayTag Call back Delegate To return Attributes Property
	/*
	* 1. using FAttributeFuncPtr = TBaseStaticDelegateInstance<FGameplayAttribute(), FDefaultDelegateUserPolicy>::FFuncPtr;
		TMap<FGameplayTag, FAttributeFuncPtr> Map_TagsToAttributes;
		
	* 2. template <class T>
		using TAttributeFuncPtr = typename TBaseStaticDelegateInstance<T, FDefaultDelegateUserPolicy>::FFuncPtr;
		TMap<FGameplayTag, TAttributeFuncPtr<FGameplayAttribute()>> Map_TagsToAttributes;
	 */
	TMap<FGameplayTag, FGameplayAttribute(*)()> Map_TagsToAttributes;

private:
	bool bTopOffHealth { false };
	bool bTopOffMana { false };
	
	/* Function */
public:
	UAuraAttributeSet();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Primary
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldValue) const;

	// Secondary
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;

	// Resistance
	UFUNCTION()
	void OnRep_FireResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_LightningResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_ArcaneResistance(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_PhysicalResistance(const FGameplayAttributeData& OldValue) const;

	// Vital
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;

	void HandleInComingDamage(const FEffectProperties& EffectProp);
	void HandleInComingXP(const FEffectProperties& EffectProp);
	static void DeBuff(const FEffectProperties& EffectProp);
	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProp);
	static void ShowFloatingText(const FEffectProperties& EffectProp, const float Damage, const bool IsBlockedHit, const bool IsCriticalHit);
	static void SendXPEvent(const FEffectProperties& EffectProp);
};