// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Untils/AuraAbilityTypes.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GAS_AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<USceneComponent> HomingTargetSceneComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Aura")
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, Category="Aura")
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere, Category="Aura")
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere, Category="Aura")
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UAudioComponent> LoopingSoundComponent;

	UPROPERTY(EditAnywhere, Category="Aura")
	float LifeTime { 10.f };

	bool bIsHit { false };

public:
	UPROPERTY(BlueprintReadWrite, Category="Aura", meta=(ExposeOnSpawn=true, AllowPrivateAccess=true))
	FDamageEffectParams DamageEffectParams;
	
	/* Function */
public:
	AAuraProjectile();

	// Get
	FORCEINLINE TObjectPtr<USceneComponent> GetHomingTargetSceneComponent() const { return HomingTargetSceneComponent; }
	FORCEINLINE TObjectPtr<UProjectileMovementComponent> GetProjectileMovementComponent() const { return ProjectileMovementComponent; }

	// Set
	FORCEINLINE void SetHomingTargetSceneComponent(const TObjectPtr<USceneComponent> InHomingTarget) { HomingTargetSceneComponent = InHomingTarget; }

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION()
	bool IsValidOverlap(const AActor* OtherActor) const;
	
	UFUNCTION(BlueprintCallable, Category="Aura")
	void OnHit();
	
	UFUNCTION()
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
