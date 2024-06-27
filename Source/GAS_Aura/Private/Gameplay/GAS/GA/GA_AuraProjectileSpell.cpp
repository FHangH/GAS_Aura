// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraProjectileSpell.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

void UGA_AuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                              const FGameplayAbilityActorInfo* ActorInfo,
                                              const FGameplayAbilityActivationInfo ActivationInfo,
                                              const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGA_AuraProjectileSpell::SpawnProjectile(
	const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch, const float PitchOverride)
{
	if (!GetAvatarActorFromActorInfo()->HasAuthority()) return;
	const auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	
	if (ProjectileClass && CombatInterface)
	{
		FTransform Transform;
		const auto SocketLocation =
			CombatInterface->Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);

		auto Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if (bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		
		Transform.SetLocation(SocketLocation);
		Transform.SetRotation(Rotation.Quaternion());

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

FString UGA_AuraProjectileSpell::GetDescription(const int32 Level)
{
	const auto Damage = static_cast<int32>(DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level));
	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches a bolt of fire, Exploding on Impact and Dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), Damage, Level);
	}
	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d bolts of fire, Exploding on Impact and Dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
}

FString UGA_AuraProjectileSpell::GetNextLevelDescription(const int32 Level)
{
	const auto Damage = static_cast<int32>(DamageTypes[FAuraGameplayTags::Get().Damage_Fire].GetValueAtLevel(Level));
	return FString::Printf(TEXT("<Title>NEXT LEVEL: </>\n\n<Default>Launches %d bolts of fire, Exploding on Impact and Dealing: </><Damage>%d</><Default> Fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectiles), Damage, Level);
}
