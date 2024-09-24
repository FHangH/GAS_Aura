// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"
#include "Gameplay/GAS/NiagaraComponent/NiagaraComponent_DeBuff.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UGameplayAbility;
class UGameplayEffect;
class UAttributeSet;
class UAbilitySystemComponent;

UCLASS(Abstract)
class GAS_AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	// Components
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|Weapon")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|NiagaraComponent")
	TObjectPtr<UNiagaraComponent_DeBuff> NiagaraComponent_DeBuff_Burn;

	// Attributes
	UPROPERTY(BlueprintReadOnly, Category="Aura|AS")
	TObjectPtr<UAttributeSet> AS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE_AttributeClass")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE_AttributeClass")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributesClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE_AttributeClass")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributesClass;

	// Type
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|CharacterType")
	ECharacterClassType CharacterClassType {ECharacterClassType::ECT_Warrior};

	// Abilities
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AbilityClass")
	TArray<TSubclassOf<UGameplayAbility>> StartUpAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|AbilityClass")
	TArray<TSubclassOf<UGameplayAbility>> StartUpPassiveAbilities;

	// SocketNames
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName WeaponTipSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName LeftHandSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName RightHandSocketName {};
	UPROPERTY(EditAnywhere, Category="Aura|Weapon|Socket")
	FName TailSocketName {};

	// Montages
	UPROPERTY(EditAnywhere, Category="Aura|Montage")
	TObjectPtr<UAnimMontage> HitReactMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Montage|Attack")
	TArray<FTaggedMontage> AttackMontages;

	// Dynamic Materials
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Material")
	TObjectPtr<UMaterialInstance> BodyDissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Material")
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;

	// Niagara System
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Niagra")	
	TObjectPtr<UNiagaraSystem> BloodEffect;

	// Sounds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Sound")	
	TObjectPtr<USoundBase> DeathSound;

	// Walk Speed
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Property", meta=(AllowPrivateAccess=true))
	float BaseWalkSpeed { 600.f };
	
	// Status
	UPROPERTY(BlueprintReadWrite, Category="Aura|Status")
	bool bIsDead { false };
	
	UPROPERTY(BlueprintReadWrite, Replicated, Category="Aura|Status")
	bool bInShockLoop { false };

	UPROPERTY(BlueprintReadWrite, ReplicatedUsing="OnRep_IsStunned", Category="Aura|Status")
	bool bIsStunned { false };

	// Minions
	int32 MinionsCount { 0 };

	// Combat Interface
	FOnASComponentRegisteredSignature OnASComponentRegisteredDelegate;
	FOnDeathSignature OnDeathDelegate;
	
	/* Function */
public:
	AAuraCharacterBase();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// Combat Interface
	virtual FOnASComponentRegisteredSignature GetOnAsComponentRegisteredDelegate() override;
	virtual FOnDeathSignature GetOnDeathDelegate() override;
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& MontageTag) const override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual TArray<FTaggedMontage> GetAttackMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() const override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	virtual int32 GetMinionCount_Implementation() const override;
	virtual void IncrementMinionCount_Implementation(const int32 Amount) override;
	virtual ECharacterClassType GetCharacterClassType_Implementation() override;
	virtual USkeletalMeshComponent* GetWeaponMesh_Implementation() const override;

	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect> GameplayEffectClass, const float Level) const;
	virtual void InitializeDefaultAttributes() const;
	void AddCharacterAbilities() const;

public:
	UAttributeSet* GetAttributeSet() const { return AS; }
	
	UFUNCTION(NetMulticast, Reliable, Category="Aura")
	void Multicast_HandleDeath(const FVector& DeathImpulse);
	
	void Dissolve();
	UFUNCTION(BlueprintNativeEvent, Category="Aura|Material")
	void StartBodyDissolveTimeLine(UMaterialInstanceDynamic* DynamicMatIns);
	UFUNCTION(BlueprintNativeEvent, Category="Aura|Material")
	void StartWeaponDissolveTimeLine(UMaterialInstanceDynamic* DynamicMatIns);

protected:
	virtual void StunTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);

	// ReplicatedUsing
	UFUNCTION()
	virtual void OnRep_IsStunned();
};