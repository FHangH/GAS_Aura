// Copyright fangh.space


#include "Gameplay/Actor/AuraProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraCollision.h"
#include "Untils/AuraLog.h"

AAuraProjectile::AAuraProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovementComponent->InitialSpeed = 550.f;
	ProjectileMovementComponent->MaxSpeed = 550.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
}

void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeTime);
	SetReplicateMovement(true);

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereStartOverlap);

	if (LoopingSound)
	{
		LoopingSoundComponent = UGameplayStatics::SpawnSoundAttached(LoopingSound, RootComponent);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("No Looping Sound set on %s"), *GetName());
    }
}

void AAuraProjectile::Destroyed()
{
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	if (!bIsHit && !HasAuthority()) OnHit();
	
	Super::Destroyed();
}

bool AAuraProjectile::IsValidOverlap(const AActor* OtherActor) const
{
	if (DamageEffectParams.SourceASComponent == nullptr) return false;

	const auto SourceAvatarActor = DamageEffectParams.SourceASComponent->GetAvatarActor();
	if (SourceAvatarActor == OtherActor) return false;

	if (!UAuraAbilitySystemFuncLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return false;
	return true;
}

void AAuraProjectile::OnHit()
{
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), GetActorRotation());
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("No Impact Effect set on %s"), *GetName());
	}
	
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation());
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("No Impact Sound set on %s"), *GetName());
	}
		
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}

	bIsHit = true;
}

void AAuraProjectile::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!DamageEffectParams.SourceASComponent) return;
	if (!IsValidOverlap(OtherActor)) return;
	if (!bIsHit) OnHit();
	
	if (HasAuthority())
	{
		if (const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			DamageEffectParams.DeathImpulse = DeathImpulse;

			if (FMath::RandRange(1, 100) < DamageEffectParams.KnockBackChance)
			{
				auto Rotation = GetActorRotation();
				Rotation.Pitch = 45.f;
				const auto KnockBackDirection = Rotation.Vector();
				DamageEffectParams.KnockBackForce = KnockBackDirection * DamageEffectParams.KnockBackForceMagnitude;
			}
			DamageEffectParams.TargetASComponent = TargetASC;
			UAuraAbilitySystemFuncLibrary::ApplyDamageEffect(DamageEffectParams);
		}
		Destroy();
	}
	else
	{
		bIsHit = true;
	}
	
	if (const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
		DamageEffectParams.DeathImpulse = DeathImpulse;

		if (FMath::RandRange(1, 100) < DamageEffectParams.KnockBackChance)
		{
			auto Rotation = GetActorRotation();
			Rotation.Pitch = 45.f;
			const auto KnockBackDirection = Rotation.Vector();
			DamageEffectParams.KnockBackForce = KnockBackDirection * DamageEffectParams.KnockBackForceMagnitude;
		}
		DamageEffectParams.TargetASComponent = TargetASC;
		UAuraAbilitySystemFuncLibrary::ApplyDamageEffect(DamageEffectParams);
	}
	Destroy();
}