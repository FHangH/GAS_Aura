// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraFireBolt.h"
#include "Gameplay/Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraLog.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UGA_AuraFireBolt::GetDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
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
			, Level, ManaCost, Cooldown, static_cast<int32>(ScaledDamage));
	}
	return FString::Printf(TEXT(
			"<Title>FIRE BOLT</>\n"
			"<Small>Level: </><Level>%d</>\n"
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			"<Small>Cooldown: </><Cooldown>%.1f</>\n"
			"<Default>Launches %d bolt of fire, Exploding on Impact and Dealing: </>"
			"<Damage>%d</><Default> Fire damage with a chance to burn</>\n")
			, Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), static_cast<int32>(ScaledDamage));
}

FString UGA_AuraFireBolt::GetNextLevelDescription(const int32 Level)
{
	const auto ScaledDamage = Damage.GetValueAtLevel(Level);
	const auto ManaCost = FMath::Abs(GetManaCost(Level));
	const auto Cooldown = GetCooldown(Level);
	
	return FString::Printf(TEXT(
		"<Title>NEXT LEVEL: </>\n"
		"<Small>Level: </><Level>%d</>\n"
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		"<Small>Cooldown: </><Cooldown>%.1f</>\n"
		"<Default>Launches %d bolts of fire, Exploding on Impact and Dealing: </>"
		"<Damage>%d</><Default> Fire damage with a chance to burn</>\n")
		, Level, ManaCost, Cooldown, FMath::Min(Level, NumProjectiles), static_cast<int32>(ScaledDamage));
}

void UGA_AuraFireBolt::SpawnProjectiles(
	const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch, const float PitchOverride, AActor* HomingTarget)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	
	const auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if (!ProjectileClass || !CombatInterface)
	{
		UE_LOG(Aura, Warning, TEXT("ProjectileClass is null in %s"), *GetName());
		return;
	}
	
	const auto SocketLocation =
		CombatInterface->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
	auto Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}

	const auto Forward = Rotation.Vector();
	const auto EffectiveNumProjectiles = FMath::Min(NumProjectiles, GetAbilityLevel());
	const auto Rotations = UAuraAbilitySystemFuncLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, EffectiveNumProjectiles);

	for (const auto& Rot : Rotations)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rot.Quaternion());

		const auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		
		if (!Projectile) return;
		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->GetProjectileMovementComponent()->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->SetHomingTargetSceneComponent(NewObject<USceneComponent>(USceneComponent::StaticClass()));
			Projectile->GetHomingTargetSceneComponent()->SetWorldLocation(ProjectileTargetLocation);
			Projectile->GetProjectileMovementComponent()->HomingTargetComponent = Projectile->GetHomingTargetSceneComponent();
		}
		Projectile->GetProjectileMovementComponent()->HomingAccelerationMagnitude = FMath::RandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->GetProjectileMovementComponent()->bIsHomingProjectile = IsLaunchHomingProjectiles;

		Projectile->FinishSpawning(SpawnTransform);
	}
}
