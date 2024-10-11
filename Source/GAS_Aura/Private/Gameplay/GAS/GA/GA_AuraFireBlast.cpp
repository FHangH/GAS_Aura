// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraFireBlast.h"
#include "Gameplay/Actor/AuraFireBall.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraLog.h"

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
	if (!IsValid(FireBallClass))
	{
		UE_LOG(Aura, Warning, TEXT("FireBallClass is not valid"));
		return TArray<AAuraFireBall*>();
	}

	TArray<AAuraFireBall*> FireBalls;
	const auto Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const auto Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	auto Rotators = UAuraAbilitySystemFuncLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const auto& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());

		auto FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		if (!FireBall) continue;
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->FinishSpawning(SpawnTransform);
		FireBalls.Add(FireBall);
	}
	return FireBalls;
}
