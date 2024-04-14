// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraProjectileSpell.h"
#include "Gameplay/Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"

void UGA_AuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_AuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	const auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	if (ProjectileClass && CombatInterface)
	{
		FTransform Transform;
		auto Rotator =
			(ProjectileTargetLocation - CombatInterface->GetCombatSocketLocation()).Rotation();
		Rotator.Pitch = 0.f;
		
		Transform.SetLocation(CombatInterface->GetCombatSocketLocation());
		Transform.SetRotation(Rotator.Quaternion());

		const auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			Transform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

		if (!Projectile) return;
		Projectile->FinishSpawning(Transform);
	}
}
