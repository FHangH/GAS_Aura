// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraFireBolt.h"
#include "Untils/AuraGameplayTags.h"

FString UGA_AuraFireBolt::GetDescription(const int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches a bolt of fire, Exploding on Impact and Dealing: </>"
			"<Damage>%d</><Default> Fire damage with a chance to burn</>\n")
			, Level, ManaCost, Cooldown, Damage);
	}
	return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d bolt of fire, Exploding on Impact and Dealing: </>"
			"<Damage>%d</><Default> Fire damage with a chance to burn</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
}

FString UGA_AuraFireBolt::GetNextLevelDescription(const int32 Level)
{
	const int32 Damage = GetDamageByDamageType(Level, FAuraGameplayTags::Get().Damage_Fire);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		"<Default>Launches %d bolts of fire, Exploding on Impact and Dealing: </>"
		"<Damage>%d</><Default> Fire damage with a chance to burn</>\n")
		, Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), Damage);
}
