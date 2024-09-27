// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UAuraAbilitySystemComponent;
class AAuraPlayerState;
class USpringArmComponent;
class UCameraComponent;
class UNiagaraComponent;

#define CHECK_PLAYER_STATE(PlayerState) (PlayerState = (PlayerState == nullptr ? GetPlayerState<AAuraPlayerState>() : PlayerState))
#define CHECK_ABILITY_SYSTEM_COMPONENT(AbilitySystemComponent) (AbilitySystemComponent = (AbilitySystemComponent == nullptr ? Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()) : AbilitySystemComponent))

UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura")
	TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura")
	TObjectPtr<USpringArmComponent> TopDownSpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura")
	TObjectPtr<UCameraComponent> TopDownCameraComponent;

private:
	TObjectPtr<AAuraPlayerState> AuraPlayerState;
	TObjectPtr<UAuraAbilitySystemComponent> AuraASComponent;
	
	/* Function */
public:
	AAuraCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	virtual void InitAbilityActorInfo() override;

	// Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;

	// Player Interface
	virtual int32 FindLevelForXP_Implementation(const int32 InXP) override;
	virtual int32 GetXP_Implementation() override;
	virtual int32 GetAttributePointReward_Implementation(const int32 Level) override;
	virtual int32 GetSpellPointReward_Implementation(const int32 Level) override;
	virtual int32 GetAttributePoints_Implementation() override;
	virtual int32 GetSpellPoints_Implementation() override;
	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual void AddToPlayerLevel_Implementation(const int32 InPlayerLevel) override;
	virtual void AddToAttributePoints_Implementation(const int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(const int32 InSpellPoints) override;
	virtual void LevelUp_Implementation() override;

private:
	// RPC
	UFUNCTION(NetMulticast, Reliable)
	void MultiCast_LevelUpParticle();

	// ReplicatedUsing
	virtual void OnRep_IsStunned() override;
	virtual void OnRep_IsBurned() override;
};
