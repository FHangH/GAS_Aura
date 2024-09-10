// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GA_AuraProjectileSpell.h"
#include "GA_AuraFireBolt.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraFireBolt : public UGA_AuraProjectileSpell
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, Category="Aura")
	float ProjectileSpread { 90.f };

	UPROPERTY(EditAnywhere, Category="Aura")
	int32 MaxNumProjectiles { 5 };

	UPROPERTY(EditAnywhere, Category="Aura")
	float HomingAccelerationMin { 3200.f };

	UPROPERTY(EditAnywhere, Category="Aura")
	float HomingAccelerationMax { 6400.f };

	UPROPERTY(EditAnywhere, Category="Aura")
	bool IsLaunchHomingProjectiles { true };
	
	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;

	UFUNCTION(BlueprintCallable, Category="Aura")
	void SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch = false, const float PitchOverride = 0.f, AActor* HomingTarget = nullptr);
};
