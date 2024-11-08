// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UDataAsset_LevelUpInfo;
class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChangedDelegate, int32 /*StateValue*/);

UCLASS()
class GAS_AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AS;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level, Category="Aura")
	int32 Level { 1 };

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP, Category="Aura")
	int32 XP { 1 };

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints, Category="Aura")
	int32 AttributePoints { 0 };
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_SpellPoints, Category="Aura")
	int32 SpellPoints { 0 };
	
public:
	// Delegate
	FOnPlayerStateChangedDelegate OnLevelChangedDelegate;
	FOnPlayerStateChangedDelegate OnXPChangedDelegate;
	FOnPlayerStateChangedDelegate OnAttributePointChangedDelegate;
	FOnPlayerStateChangedDelegate OnSpellPointChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Aura")
	TObjectPtr<UDataAsset_LevelUpInfo> DA_LevelUpInfo;
	
	/* Function */
public:
	AAuraPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Inline Get
	FORCEINLINE UAttributeSet* GetAttributeSet() const { return AS; }
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetPlayerXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }

	// Add
	void AddLevel(const int32 InLevel);
	void AddToXP(const int32 InXP);
	void AddToAttributePoints(const int32 InAttributePoints);
	void AddToSpellPoints(const int32 InSpellPoints);

	// Set
	void SetLevel(const int32 InLevel);
	void SetXP(const int32 InXP);
	void SetAttributePoints(const int32 InAttributePoints);
	void SetSpellPoints(const int32 InSpellPoints);

	// On RepNotify
	UFUNCTION()
	void OnRep_Level(const int32 OldLevel) const;
	UFUNCTION()
	void OnRep_XP(const int32 OldXP) const;
	UFUNCTION()
	void OnRep_AttributePoints(const int32 OldAttributePoint) const;
	UFUNCTION()
	void OnRep_SpellPoints(const int32 OldSpellPoint) const;
};
