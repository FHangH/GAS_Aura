// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "GA_AuraBeamSpell.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraBeamSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadWrite, Category="Aura|Beam")
	FVector MouseHitLocation;

	UPROPERTY(BlueprintReadWrite, Category="Aura|Beam")
	TObjectPtr<AActor> MouseHitActor;

	UPROPERTY(BlueprintReadWrite, Category="Aura|Beam")
	TObjectPtr<APlayerController> OwnerPlayerController;

	UPROPERTY(BlueprintReadWrite, Category="Aura|Beam")
	TObjectPtr<ACharacter> OwnerCharacter;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void StoreMouseDataInfo(const FHitResult& HitResult);

	UFUNCTION(BlueprintCallable, Category="Aura")
	void StoreOwners();

	UFUNCTION(BlueprintCallable, Category="Aura")
	void TraceFirstTarget(const FVector& BeamTargetLocation);
};