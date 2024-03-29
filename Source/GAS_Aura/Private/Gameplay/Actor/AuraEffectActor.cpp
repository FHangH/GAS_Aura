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
		TargetASC->ApplyGameplayEffectSpecToSelf(*GESpecHandle.Data.Get());
	}
}
