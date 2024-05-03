// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * Aura GameplayTags
 * Singleton Containing Native Gameplay Tag
 */

struct FAuraGameplayTags
{
 /* Property */
private:
 static FAuraGameplayTags GameplayTags;

public:
 // Attributes Tags
 FGameplayTag Attributes_Primary_Strength;
 FGameplayTag Attributes_Primary_Intelligence;
 FGameplayTag Attributes_Primary_Resilience;
 FGameplayTag Attributes_Primary_Vigor;
 
 FGameplayTag Attributes_Secondary_MaxHealth;
 FGameplayTag Attributes_Secondary_MaxMana;
 FGameplayTag Attributes_Secondary_Armor;
 FGameplayTag Attributes_Secondary_ArmorPenetration;
 FGameplayTag Attributes_Secondary_BlockChance;
 FGameplayTag Attributes_Secondary_CriticalHitChance;
 FGameplayTag Attributes_Secondary_CriticalHitDamage;
 FGameplayTag Attributes_Secondary_CriticalHitResistance;
 FGameplayTag Attributes_Secondary_HealthRegeneration;
 FGameplayTag Attributes_Secondary_ManaRegeneration;

 FGameplayTag Attributes_Resistance_Fire;
 FGameplayTag Attributes_Resistance_Lightning;
 FGameplayTag Attributes_Resistance_Arcane;
 FGameplayTag Attributes_Resistance_Physical;

 // InputAction Tags
 FGameplayTag InputTag_LMB; // Left Mouse Button
 FGameplayTag InputTag_RMB; // Right Mouse Button
 FGameplayTag InputTag_1;
 FGameplayTag InputTag_2;
 FGameplayTag InputTag_3;
 FGameplayTag InputTag_4;

 // Damage Tags
 FGameplayTag Damage;
 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Lightning;
 FGameplayTag Damage_Arcane;
 FGameplayTag Damage_Physical;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;

 // HitReact Tag
 FGameplayTag Effects_HitReact;

 // Ability Tags
 FGameplayTag Ability_Attack;

 // Montage Tags
 FGameplayTag Montage_Attack_Weapon;
 FGameplayTag Montage_Attack_LeftHand;
 FGameplayTag Montage_Attack_RightHand;
 
 /* Function */
public:
 static const FAuraGameplayTags& Get() { return GameplayTags; }
 static void InitNativeGameplayTags();
};