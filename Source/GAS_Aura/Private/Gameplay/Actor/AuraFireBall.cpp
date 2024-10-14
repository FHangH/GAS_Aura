// Copyright fangh.space


#include "Gameplay/Actor/AuraFireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"

AAuraFireBall::AAuraFireBall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraFireBall::StartOutgoingTimeLine_Implementation()
{
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeLine();
}

void AAuraFireBall::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValidOverlap(OtherActor)) return;
	if (!HasAuthority()) return;
	
	if (const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		const auto DeathImpulse = GetActorForwardVector() * DamageEffectParams.DeathImpulseMagnitude;
			
		DamageEffectParams.DeathImpulse = DeathImpulse;
		DamageEffectParams.TargetASComponent = TargetASC;
		UAuraAbilitySystemFuncLibrary::ApplyDamageEffect(DamageEffectParams);
	}
}