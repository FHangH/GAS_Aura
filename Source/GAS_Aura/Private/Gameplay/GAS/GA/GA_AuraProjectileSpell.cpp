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
	
        if (DamageEffectClass)
        {
	        if (const auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
        	{
		        const auto SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());
	        	const auto ScaledDamage = Damage.GetValueAtLevel(/*GetAbilityLevel()*/10.f);
	        	
	        	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, FAuraGameplayTags::Get().Damage, ScaledDamage);
        		Projectile->DamageEffectSpecHandle = SpecHandle;
        	}
        }
        else
        {
	        UE_LOG(Aura, Warning, TEXT("Damage Effect Class is null in %s"), *GetName());
        }
		
		if (!Projectile) return;
		Projectile->FinishSpawning(Transform);
	}
}
