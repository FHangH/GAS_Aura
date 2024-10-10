// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraArcaneShards.h"

FString UGA_AuraArcaneShards::GetDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ARCANE SHARDS</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Summon shards of arcane energy, Causing radial arcane damage of: </>"
			"<Damage>%d</><Default> At the shard origin.</>\n")
			, Level, ManaCost, Cooldown, static_cast<int32>(ScaledDamage));
	}
	return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Summon %d shards of arcane energy, Causing radial arcane damage of: </>"
			"<Damage>%d</><Default> At the shard origin.</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), static_cast<int32>(ScaledDamage));
}

FString UGA_AuraArcaneShards::GetNextLevelDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL: </>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Summon %d shards of arcane energy, Causing radial arcane damage of: </>"
			"<Damage>%d</><Default> At the shard origin.</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), static_cast<int32>(ScaledDamage));
}
