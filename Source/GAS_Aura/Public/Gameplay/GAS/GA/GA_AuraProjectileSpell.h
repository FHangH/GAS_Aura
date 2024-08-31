// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "GA_AuraProjectileSpell.generated.h"

class AAuraProjectile;

UCLASS()
class GAS_AURA_API UGA_AuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura", meta=(AllowPrivateAccess=true))
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura", meta=(AllowPrivateAccess=true))
	int32 NumProjectiles { 5 };
	
	/* Function */
public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Aura")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, const bool bOverridePitch = false, const float PitchOverride = 0.f);
};
