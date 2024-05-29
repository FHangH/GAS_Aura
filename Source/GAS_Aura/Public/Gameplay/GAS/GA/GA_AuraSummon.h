// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraGameplayAbility.h"
#include "GA_AuraSummon.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraSummon : public UAuraGameplayAbility
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	int32 NumMinions {5};

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	float MinSpawnDistance {50.f};
	
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	float MaxSpawnDistance {250.f};

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	float SpawnSpread {90.f};

	/* Function */
public:
	UFUNCTION(BlueprintPure, Category="Aura")
	TArray<FVector> GetSpawnLocations();

	void DebugSpawnLocations(const FVector& SpawnLocation, const FVector& StartLocation, const FVector& SpawnDirection) const;
};
