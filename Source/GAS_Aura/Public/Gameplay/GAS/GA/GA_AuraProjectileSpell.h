// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraGameplayAbility.h"
#include "GA_AuraProjectileSpell.generated.h"

class AAuraProjectile;

UCLASS()
class GAS_AURA_API UGA_AuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura", meta=(AllowPrivateAccess=true))
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
	/* Function */
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category="Aura")
	void SpawnProjectile();
};
