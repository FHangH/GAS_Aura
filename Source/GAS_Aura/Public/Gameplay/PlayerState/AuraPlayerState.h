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
	int32 Level {1};

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP, Category="Aura")
	int32 XP {1};

public:
	// Delegate
	FOnPlayerStateChangedDelegate OnLevelChangedDelegate;
	FOnPlayerStateChangedDelegate OnXPChangedDelegate;

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

	// Add
	void AddLevel(const int32 InLevel);
	void AddToXP(const int32 InXP);

	// Set
	void SetLevel(const int32 InLevel);
	void SetXP(const int32 InXP);

	UFUNCTION()
	void OnRep_Level(const int32 OldLevel) const;

	UFUNCTION()
	void OnRep_XP(const int32 OldXP) const;
};
