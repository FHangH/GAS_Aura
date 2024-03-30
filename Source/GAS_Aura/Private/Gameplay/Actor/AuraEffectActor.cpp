// Copyright fangh.space


#include "Gameplay/Actor/AuraEffectActor.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComp")));
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		auto GEContextHandle = TargetASC->MakeEffectContext();
		GEContextHandle.AddSourceObject(this);
		
		const auto GESpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, GEContextHandle);
		const auto ActiveGEHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
		const auto bIsInfinite = GESpecHandle.Data.Get()->Def.Get()->DurationPolicy == EGameplayEffectDurationType::Infinite;

		if (bIsInfinite && InfinityGE_RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
		{
			ActiveGEHandles_Map.Add(ActiveGEHandle, TargetASC);
		}
 	}
}

void AAuraEffectActor::OnBeginOverlap(AActor* TargetActor)
{
	if (InstantGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfinityGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnBeginOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
	}
}

void AAuraEffectActor::OnEndOverlap(AActor* TargetActor)
{
	if (InstantGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClass);
	}
	if (DurationGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClass);
	}
	if (InfinityGE_ApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfinityGameplayEffectClass);
	}
	if (InfinityGE_RemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if (!TargetASC) return;

		TArray<FActiveGameplayEffectHandle> HandlesToRemove;
		for (const auto& GEHandlePair : ActiveGEHandles_Map)
		{
			if (TargetASC == GEHandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(GEHandlePair.Key, 1);
				//ActiveGEHandles_Map.FindAndRemoveChecked(GEHandlePair.Key);
			}
		}
		for (const auto& Handle : HandlesToRemove)
		{
			ActiveGEHandles_Map.FindAndRemoveChecked(Handle);
		}
	}
}
