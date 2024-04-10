#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
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

void AAuraCharacterBase::ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const
{
	if (ASComponent)
	{
		auto GEContextHandle = ASComponent->MakeEffectContext();
		GEContextHandle.AddSourceObject(this);
		const auto GESpecHandle = ASComponent->MakeOutgoingSpec(GameplayEffectClass, Level, GEContextHandle);
		ASComponent->ApplyGameplayEffectSpecToTarget(*GESpecHandle.Data.Get(), ASComponent);
	}
}

void AAuraCharacterBase::InitializeDefaultAttributes() const
{
	if (DefaultPrimaryAttributesClass)
	{
		ApplyEffectToSelf(DefaultPrimaryAttributesClass, 1.f);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultPrimaryAttributesClass Is Null"));
	}
	
	if (DefaultSecondaryAttributesClass)
	{
		ApplyEffectToSelf(DefaultSecondaryAttributesClass, 1.f);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultSecondaryAttributesClass Is Null"));
	}

	if (DefaultVitalAttributesClass)
	{
		ApplyEffectToSelf(DefaultVitalAttributesClass, 1.f);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultVitalAttributesClass Is Null"));
	}
}

void AAuraCharacterBase::AddCharacterAbilities() const
{
	if (!HasAuthority()) return;

	const auto ASC = CastChecked<UAuraAbilitySystemComponent>(ASComponent);
	if (!ASC) return;

	ASC->AddCharacterAbilities(StartUpAbilities);
}
