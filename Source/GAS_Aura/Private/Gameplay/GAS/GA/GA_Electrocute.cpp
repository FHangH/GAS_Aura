// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_Electrocute.h"

FString UGA_Electrocute::GetDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	if (Level == 1)
	{
		return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning, Connecting with the target, Repeatedly causing: </>"
			"<Damage>%d</><Default> Lightning damage with a chance to stun.</>\n")
			, Level, ManaCost, Cooldown, static_cast<int32>(ScaledDamage));
	}
	return FString::Printf(TEXT(
			"<Title>ELECTROCUTE</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning, Propagating to %d additional targets nearby, Causing: </>"
			"<Damage>%d</><Default> Lightning damage with a chance to stun.</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), static_cast<int32>(ScaledDamage));
}

FString UGA_Electrocute::GetNextLevelDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL: </>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Emits a beam of lightning, Propagating to %d additional targets nearby, Causing: </>"
			"<Damage>%d</><Default> Lightning damage with a chance to stun.</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), static_cast<int32>(ScaledDamage));
}
