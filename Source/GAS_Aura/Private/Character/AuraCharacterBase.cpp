// Copyright fangh.space


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Untils/AuraCollision.h"
#include "Untils/AuraLog.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

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

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation() const 
{
	return WeaponMeshComponent ? WeaponMeshComponent->GetSocketLocation(WeaponTipSocketName) : FVector{};
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die()
{
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	Multicast_HandleDeath();
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
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
	if (IsValid(DefaultPrimaryAttributesClass))
	{
		ApplyEffectToSelf(DefaultPrimaryAttributesClass, 1.f);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultPrimaryAttributesClass Is Null"));
	}
	
	if (IsValid(DefaultSecondaryAttributesClass))
	{
		ApplyEffectToSelf(DefaultSecondaryAttributesClass, 1.f);
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("AuraCharacterBase DefaultSecondaryAttributesClass Is Null"));
	}

	if (IsValid(DefaultVitalAttributesClass))
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

void AAuraCharacterBase::Multicast_HandleDeath_Implementation()
{
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->SetSimulatePhysics(true);
		WeaponMeshComponent->SetEnableGravity(true);
		WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	bIsDead = true;
	Dissolve();
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(BodyDissolveMaterialInstance) && GetMesh())
	{
		auto DynamicMatIns = UMaterialInstanceDynamic::Create(BodyDissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatIns);
		StartBodyDissolveTimeLine(DynamicMatIns);
	}
	if (IsValid(WeaponDissolveMaterialInstance) && WeaponMeshComponent)
	{
		auto DynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		WeaponMeshComponent->SetMaterial(0, DynamicMatIns);
		StartWeaponDissolveTimeLine(DynamicMatIns);
	}
}

void AAuraCharacterBase::StartBodyDissolveTimeLine_Implementation(UMaterialInstanceDynamic* DynamicMatIns)
{
}

void AAuraCharacterBase::StartWeaponDissolveTimeLine_Implementation(UMaterialInstanceDynamic* DynamicMatIns)
{
}
