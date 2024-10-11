// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraProjectile.h"
#include "AuraFireBall.generated.h"

UCLASS()
class GAS_AURA_API AAuraFireBall : public AAuraProjectile
{
	GENERATED_BODY()

public:
	AAuraFireBall();

protected:
	virtual void BeginPlay() override;

	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
  };