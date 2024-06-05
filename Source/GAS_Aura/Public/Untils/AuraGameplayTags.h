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

 // Attributes.Meta Tags
 FGameplayTag Attributes_Meta_IncomingXP;

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
 FGameplayTag Ability_Summon;
 FGameplayTag Ability_Fire_FireBolt;

 // CoolDown Tags
 FGameplayTag Cooldown_Fire_FireBolt;

 // CombatSocket Tags
 FGameplayTag CombatSocket_Weapon;
 FGameplayTag CombatSocket_LeftHand;
 FGameplayTag CombatSocket_RightHand;
 FGameplayTag CombatSocket_Tail;

 // Montage Tags
 FGameplayTag Montage_Attack_1;
 FGameplayTag Montage_Attack_2;
 FGameplayTag Montage_Attack_3;
 FGameplayTag Montage_Attack_4;
 
 /* Function */
public:
 static const FAuraGameplayTags& Get() { return GameplayTags; }
 static void InitNativeGameplayTags();
};