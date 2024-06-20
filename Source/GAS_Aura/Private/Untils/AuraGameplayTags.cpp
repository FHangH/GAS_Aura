// Copyright fangh.space


#include "Untils/AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags{};

void FAuraGameplayTags::InitNativeGameplayTags()
{
	// Attributes Tags
	// Primary
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Strength"});
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Intelligence"});
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Resilience"});
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Vigor"});
	// Secondary
	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.MaxHealth"});
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.MaxMana"});
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.Armor"});
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.ArmorPenetration"});
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.BlockChance"});
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitChance"});
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitDamage"});
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitResistance"});
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.HealthRegeneration"});
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.ManaRegeneration"});
	// Resistance
	GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Resistance.Fire"});
	GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Resistance.Lightning"});
	GameplayTags.Attributes_Resistance_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Resistance.Arcane"});
	GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Resistance.Physical"});
	// Meta
	GameplayTags.Attributes_Meta_IncomingXP = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Meta.IncomingXP"});
	
	// InputAction Tags
	GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.LMB"});
	GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.RMB"});
	GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.1"});
	GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.2"});
	GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.3"});
	GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.4"});
	GameplayTags.InputTag_Passive_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.Passive.1"});
	GameplayTags.InputTag_Passive_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"InputTag.Passive.2"});

	// Damage Tags
	GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Damage"});
	GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Damage.Fire"});
	GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Damage.Lightning"});
	GameplayTags.Damage_Arcane = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Damage.Arcane"});
	GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Damage.Physical"});
	
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Arcane, GameplayTags.Attributes_Resistance_Arcane);
	GameplayTags.DamageTypesToResistance.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
	
	// Hit React
	GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Effects.HitReact"});

	// Ability
	GameplayTags.Ability_Attack = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Attack"});
	GameplayTags.Ability_Summon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Summon"});
	GameplayTags.Ability_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Fire.FireBolt"});

	// Ability HitReact
	GameplayTags.Ability_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.HitReact"});
	
	// Ability Status
	GameplayTags.Ability_Status_Locked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Status.Locked"});
	GameplayTags.Ability_Status_Eligible = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Status.Eligible"});
	GameplayTags.Ability_Status_Unlocked = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Status.Unlocked"});
	GameplayTags.Ability_Status_Equipped = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Status.Equipped"});
	
	// Ability Type
	GameplayTags.Ability_Type_Offensive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Type.Offensive"});
	GameplayTags.Ability_Type_Passive = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Type.Passive"});
	GameplayTags.Ability_Type_None = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Ability.Type.None"});

	// CoolDown
	GameplayTags.Cooldown_Fire_FireBolt = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"CoolDown.Fire.FireBolt"});
	
	// CombatSocket
	GameplayTags.CombatSocket_Weapon = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"CombatSocket.Weapon"});
	GameplayTags.CombatSocket_LeftHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"CombatSocket.LeftHand"});
	GameplayTags.CombatSocket_RightHand = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"CombatSocket.RightHand"});
	GameplayTags.CombatSocket_Tail = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"CombatSocket.Tail"});

	// Montage
	GameplayTags.Montage_Attack_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Montage.Attack.1"});
	GameplayTags.Montage_Attack_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Montage.Attack.2"});
	GameplayTags.Montage_Attack_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Montage.Attack.3"});
	GameplayTags.Montage_Attack_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Montage.Attack.4"});
}
