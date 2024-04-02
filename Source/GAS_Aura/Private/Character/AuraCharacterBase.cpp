#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Untils/AuraLog.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), FName{"WeaponHandSocket"});
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
{
	return ASComponent;
}

void AAuraCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAuraCharacterBase::InitAbilityActorInfo()
{
	
}

void AAuraCharacterBase::InitializePrimaryAttributes() const
{
	if (!DefaultPrimaryAttributesClass)
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultPrimaryAttributesClass Is Null"));
		return;
	}
	if (ASComponent)
	{
		const auto GEContextHandle = ASComponent->MakeEffectContext();
		const auto GESpecHandle = ASComponent->MakeOutgoingSpec(DefaultPrimaryAttributesClass, 1.f, GEContextHandle);
		ASComponent->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), ASComponent);
	}
}
