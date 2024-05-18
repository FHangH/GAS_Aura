// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

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
		const auto SocketLocation =
			ICombatInterface::Execute_GetCombatSocketLocation(
				GetAvatarActorFromActorInfo(), FAuraGameplayTags::Get().CombatSocket_Weapon);
		const auto Rotator = (ProjectileTargetLocation - SocketLocation).Rotation();
		
		Transform.SetLocation(SocketLocation);
		Transform.SetRotation(Rotator.Quaternion());

		const auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			Transform,
			GetOwningActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	
        if (DamageEffectClass)
        {
	        if (const auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
        	{
	        	auto EffectContextHandle = SourceASC->MakeEffectContext();
	        	EffectContextHandle.SetAbility(this);
	        	EffectContextHandle.AddSourceObject(Projectile);
				TArray<TWeakObjectPtr<AActor>> Actors;
	        	Actors.Add(Projectile);
	        	EffectContextHandle.AddActors(Actors);
	        	FHitResult HitResult;
	        	HitResult.Location = ProjectileTargetLocation;
	        	EffectContextHandle.AddHitResult(HitResult);
	        	
		        const auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
				for (const auto& Pair : DamageTypes)
				{
					const auto ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
					UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
				}
        		Projectile->DamageEffectSpecHandle = SpecHandle;
        	}
        }
        else
        {
	        UE_LOG(Aura, Warning, TEXT("DamageEffectClass is null in %s"), *GetName());
        }
		
		if (!Projectile) return;
		Projectile->FinishSpawning(Transform);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("ProjectileClass is null in %s"), *GetName());
	}
}
