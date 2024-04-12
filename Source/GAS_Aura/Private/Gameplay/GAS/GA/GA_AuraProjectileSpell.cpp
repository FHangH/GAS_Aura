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

void UGA_AuraProjectileSpell::SpawnProjectile()
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	if (ProjectileClass && CombatInterface)
	{
		FTransform Transform;
		Transform.SetLocation(CombatInterface->GetCombatSocketLocation());

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
