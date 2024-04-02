// Copyright fangh.space

/**
 * This defines a set of helper functions for accessing and initializing attributes, to avoid having to manually write these functions.
 * It would creates the following functions, for attribute Health
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
	UAbilitySystemComponent* SourceASC {nullptr};
	UPROPERTY()
	AActor* SourceAvatarActor {nullptr};
	UPROPERTY()
	AController* SourceController {nullptr};
	UPROPERTY()
	ACharacter* SourceCharacter {nullptr};

	// Target
	UPROPERTY()
	UAbilitySystemComponent* TargetASC {nullptr};
	UPROPERTY()
	AActor* TargetAvatarActor {nullptr};
	UPROPERTY()
	AController* TargetController {nullptr};
	UPROPERTY()
	ACharacter* TargetCharacter {nullptr};
};

UCLASS()
class GAS_AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* Property */
public:
	// Primary Attribute
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
	
	// Vital Attribute
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Health, Category="Aura|AS|Vital")
	FGameplayAttributeData Health {50.f};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxHealth, Category="Aura|AS|Vital")
	FGameplayAttributeData MaxHealth {100.f};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_Mana, Category="Aura|AS|Vital")
	FGameplayAttributeData Mana {25.f};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Mana)

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_MaxMana, Category="Aura|AS|Vital")
	FGameplayAttributeData MaxMana {50.f};
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet, MaxMana)

	// EffectContextHandle, ASC, Avatar, Controller, Character
	UPROPERTY(BlueprintReadOnly, Category="Aura|AS")
	FEffectProperties EffectProperties {};

	/* Function */
public:
	UAuraAttributeSet();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldValue) const;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldValue) const;
	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldValue) const;

	static void SetEffectProperties(const FGameplayEffectModCallbackData& Data, FEffectProperties& EffectProperties);
};
