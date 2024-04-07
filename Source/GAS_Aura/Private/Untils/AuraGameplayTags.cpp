// Copyright fangh.space


#include "Untils/AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags{};

void FAuraGameplayTags::InitNativeGameplayTags()
{
	GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Strength"}, FString{});
	GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Intelligence"}, FString{});
	GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Resilience"}, FString{});
	GameplayTags.Attributes_Primary_Vigor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Primary.Vigor"}, FString{});

	GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.MaxHealth"}, FString{});
	GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.MaxMana"}, FString{});
	GameplayTags.Attributes_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.Armor"}, FString{});
	GameplayTags.Attributes_Secondary_ArmorPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.ArmorPenetration"}, FString{});
	GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.BlockChance"}, FString{});
	GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitChance"}, FString{});
	GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitDamage"}, FString{});
	GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.CriticalHitResistance"}, FString{});
	GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.HealthRegeneration"}, FString{});
	GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName{"Attributes.Secondary.ManaRegeneration"}, FString{});
}
