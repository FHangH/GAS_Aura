// Copyright fangh.space


#include "Character/AuraCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "Components/CapsuleComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Untils/AuraCollision.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"
#include "GameFramework/CharacterMovementComponent.h"

AAuraCharacterBase::AAuraCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Projectile, ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMeshComponent"));
	WeaponMeshComponent->SetupAttachment(GetMesh(), FName{"WeaponHandSocket"});
	WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	NiagaraComponent_DeBuff_Burn = CreateDefaultSubobject<UNiagaraComponent_DeBuff>("NiagaraComp_DeBuff_Burn");
	NiagaraComponent_DeBuff_Burn->SetupAttachment(GetRootComponent());
	NiagaraComponent_DeBuff_Burn->DeBuff_Tag = FAuraGameplayTags::Get().DeBuff_Burn;

	NiagaraComponent_DeBuff_Stun = CreateDefaultSubobject<UNiagaraComponent_DeBuff>("NiagaraComp_DeBuff_Stun");
	NiagaraComponent_DeBuff_Stun->SetupAttachment(GetRootComponent());
	NiagaraComponent_DeBuff_Stun->DeBuff_Tag = FAuraGameplayTags::Get().DeBuff_Stun;
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

void AAuraCharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraCharacterBase, bInShockLoop);
	DOREPLIFETIME(AAuraCharacterBase, bIsStunned);
	DOREPLIFETIME(AAuraCharacterBase, bIsBurned);
}

FOnASComponentRegisteredSignature& AAuraCharacterBase::GetOnAsComponentRegisteredDelegate()
{
	return OnASComponentRegisteredDelegate;
}

FOnDeathSignature& AAuraCharacterBase::GetOnDeathDelegate()
{
	return OnDeathDelegate;
}

FVector AAuraCharacterBase::GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const 
{
	const auto GameplayTags = FAuraGameplayTags::Get();

	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Weapon) && WeaponMeshComponent)
	{
		return WeaponMeshComponent->GetSocketLocation(WeaponTipSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_LeftHand))
	{
		return GetMesh()->GetSocketLocation(LeftHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_RightHand))
	{
		return GetMesh()->GetSocketLocation(RightHandSocketName);
	}
	if (MontageTag.MatchesTagExact(GameplayTags.CombatSocket_Tail))
	{
		return GetMesh()->GetSocketLocation(TailSocketName);
	}
	return FVector{};
}

UAnimMontage* AAuraCharacterBase::GetHitReactMontage_Implementation()
{
	return HitReactMontage;
}

void AAuraCharacterBase::Die(const FVector& DeathImpulse)
{
	bIsDead = true;
	
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	}
	Multicast_HandleDeath(DeathImpulse);
}

bool AAuraCharacterBase::IsDead_Implementation() const
{
	return bIsDead;
}

AActor* AAuraCharacterBase::GetAvatar_Implementation()
{
	return this;
}

TArray<FTaggedMontage> AAuraCharacterBase::GetAttackMontages_Implementation()
{
	return AttackMontages;
}

UNiagaraSystem* AAuraCharacterBase::GetBloodEffect_Implementation() const
{
	return BloodEffect;
}

FTaggedMontage AAuraCharacterBase::GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag)
{
	for (const auto& Tag : AttackMontages)
	{
		if (Tag.MontageTag == MontageTag)
		{
			return Tag;
		}
	}
	return FTaggedMontage{};
}

int32 AAuraCharacterBase::GetMinionCount_Implementation() const
{
	return MinionsCount;
}

void AAuraCharacterBase::IncrementMinionCount_Implementation(const int32 Amount)
{
	MinionsCount += Amount;
}

ECharacterClassType AAuraCharacterBase::GetCharacterClassType_Implementation()
{
	return CharacterClassType;
}

USkeletalMeshComponent* AAuraCharacterBase::GetWeaponMesh_Implementation() const
{
	return WeaponMeshComponent;
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
	ASC->AddCharacterPassiveAbilities(StartUpPassiveAbilities);
}

void AAuraCharacterBase::Multicast_HandleDeath_Implementation(const FVector& DeathImpulse)
{
	bIsDead = true;
	
	// TODO Here Delegate Is Not Bound, GA_AuraBeamSpell Function: PrimaryTargetDied and AdditionalTargetDied Can not CallBack
	OnDeathDelegate.Broadcast(this);
	
	if (WeaponMeshComponent)
	{
		WeaponMeshComponent->SetSimulatePhysics(true);
		WeaponMeshComponent->SetEnableGravity(true);
		WeaponMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		WeaponMeshComponent->AddImpulse(DeathImpulse * 0.1f, NAME_None, true);
	}
	if (GetMesh())
	{
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetEnableGravity(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
		GetMesh()->AddImpulse(DeathImpulse, NAME_None, true);
	}
	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (DeathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation(), GetActorRotation());
	}
	if (NiagaraComponent_DeBuff_Burn)
	{
		NiagaraComponent_DeBuff_Burn->Deactivate();
	}
	if (NiagaraComponent_DeBuff_Stun)
	{
		NiagaraComponent_DeBuff_Stun->Deactivate();
	}
	
	Dissolve();
}

void AAuraCharacterBase::Dissolve()
{
	if (IsValid(BodyDissolveMaterialInstance) && GetMesh())
	{
		const auto DynamicMatIns = UMaterialInstanceDynamic::Create(BodyDissolveMaterialInstance, this);
		GetMesh()->SetMaterial(0, DynamicMatIns);
		StartBodyDissolveTimeLine(DynamicMatIns);
	}
	if (IsValid(WeaponDissolveMaterialInstance) && WeaponMeshComponent)
	{
		const auto DynamicMatIns = UMaterialInstanceDynamic::Create(WeaponDissolveMaterialInstance, this);
		WeaponMeshComponent->SetMaterial(0, DynamicMatIns);
		StartWeaponDissolveTimeLine(DynamicMatIns);
	}
}

void AAuraCharacterBase::StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount)
{
	bIsStunned = NewCount > 0;
	GetCharacterMovement()->MaxWalkSpeed = bIsStunned ? 0.f : BaseWalkSpeed;
}

void AAuraCharacterBase::OnRep_IsStunned()
{
}

void AAuraCharacterBase::OnRep_IsBurned()
{
}

void AAuraCharacterBase::StartBodyDissolveTimeLine_Implementation(UMaterialInstanceDynamic* DynamicMatIns)
{
}

void AAuraCharacterBase::StartWeaponDissolveTimeLine_Implementation(UMaterialInstanceDynamic* DynamicMatIns)
{
}
