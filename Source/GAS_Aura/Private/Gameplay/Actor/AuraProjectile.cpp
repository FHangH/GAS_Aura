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
	InitialLifeSpan = LifeTime;

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

	InitialLifeSpan = LifeTime;
	SetLifeSpan(LifeTime);

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
	if (!bIsHit && !HasAuthority()) OnHit();
	
	Super::Destroyed();
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
		
	if (LoopingSoundComponent) LoopingSoundComponent->Stop();

	bIsHit = true;
}

void AAuraProjectile::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const auto SourceAvatarActor = DamageEffectParams.SourceASComponent->GetAvatarActor();

	if (SourceAvatarActor == OtherActor) return;
	if (!UAuraAbilitySystemFuncLibrary::IsNotFriend(SourceAvatarActor, OtherActor)) return;

	if (!bIsHit) OnHit();
	if (!HasAuthority())
	{
		bIsHit = true;
		return;
	}
	
	if (const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		DamageEffectParams.TargetASComponent = TargetASC;
		UAuraAbilitySystemFuncLibrary::ApplyDamageEffect(DamageEffectParams);
	}
	Destroy();
}