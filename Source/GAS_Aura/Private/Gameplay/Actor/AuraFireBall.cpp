// Copyright fangh.space


#include "Gameplay/Actor/AuraFireBall.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayCueManager.h"
#include "Components/AudioComponent.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"

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

void AAuraFireBall::OnHit()
{
	if (GetOwner())
	{
		FGameplayCueParameters CueParameters;
		CueParameters.Location = GetActorLocation();
		UGameplayCueManager::ExecuteGameplayCue_NonReplicated(GetOwner(), FAuraGameplayTags::Get().GameplayCue_FireBlast, CueParameters);
	}
	
	if (LoopingSoundComponent)
	{
		LoopingSoundComponent->Stop();
		LoopingSoundComponent->DestroyComponent();
	}
	bIsHit = true;
}
