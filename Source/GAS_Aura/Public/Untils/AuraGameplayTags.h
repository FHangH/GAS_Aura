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
 FGameplayTag InputTag_Passive_1;
 FGameplayTag InputTag_Passive_2;

 // Damage Tags
 FGameplayTag Damage;
 FGameplayTag Damage_Fire;
 FGameplayTag Damage_Lightning;
 FGameplayTag Damage_Arcane;
 FGameplayTag Damage_Physical;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToResistance;

 // Effect HitReact Tag
 FGameplayTag Effects_HitReact;

 // Ability Tags
 FGameplayTag Ability_None;
 
 FGameplayTag Ability_Attack;
 FGameplayTag Ability_Summon;
 
 FGameplayTag Ability_Fire_FireBolt;
 FGameplayTag Ability_Fire_FireBlast;
 FGameplayTag Ability_Lightning_Electrocute;
 FGameplayTag Ability_Arcane_ArcaneShards;

 FGameplayTag Ability_Passive_HaloOfProtection;
 FGameplayTag Ability_Passive_LifeSiphon;
 FGameplayTag Ability_Passive_ManaSiphon;
 
 // Ability HitReact Tag
 FGameplayTag Ability_HitReact;

 // Ability Status Tags
 FGameplayTag Ability_Status_Locked;
 FGameplayTag Ability_Status_Eligible;
 FGameplayTag Ability_Status_Unlocked;
 FGameplayTag Ability_Status_Equipped;
 
 // Ability Type Tags
 FGameplayTag Ability_Type_Offensive;
 FGameplayTag Ability_Type_Passive;
 FGameplayTag Ability_Type_None;

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

 // DeBuff Tags
 FGameplayTag DeBuff_Burn;
 FGameplayTag DeBuff_Stun;
 FGameplayTag DeBuff_Arcane;
 FGameplayTag DeBuff_Physical;
 TMap<FGameplayTag, FGameplayTag> DamageTypesToDeBuffs;

 FGameplayTag DeBuff_Chance;
 FGameplayTag DeBuff_Damage;
 FGameplayTag DeBuff_Frequency;
 FGameplayTag DeBuff_Duration;

 // Block Tags
 FGameplayTag Player_Block_InputPressed;
 FGameplayTag Player_Block_InputHeld;
 FGameplayTag Player_Block_InputReleased;
 FGameplayTag Player_Block_CursorTrace;

 // GameplayCue Tag
 FGameplayTag GameplayCue_FireBlast;
 
 /* Function */
public:
 static const FAuraGameplayTags& Get() { return GameplayTags; }
 static void InitNativeGameplayTags();
};