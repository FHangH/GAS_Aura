// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraFireBlast.h"

FString UGA_AuraFireBlast::GetDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
			"<Title>FIRE BLAST</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d fire balls in all directions, Each coming back and exploding upon return, Casusing </>"
			"<Damage>%d</><Default> Radial fire damage with a chance to burn</>\n")
			, Level, ManaCost, Cooldown, NumFireBalls, static_cast<int32>(ScaledDamage));
}

FString UGA_AuraFireBlast::GetNextLevelDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
			"<Title>NEXT LEVEL: </>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d fire balls in all directions, Each coming back and exploding upon return, Casusing </>"
			"<Damage>%d</><Default> Radial fire damage with a chance to burn</>\n")
			, Level, ManaCost, Cooldown, NumFireBalls, static_cast<int32>(ScaledDamage));
}

TArray<AAuraFireBall*> UGA_AuraFireBlast::SpawnFireBalls()
{
	return TArray<AAuraFireBall*>{};
}
