// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class GAS_AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;
	
	UPROPERTY(VisibleAnywhere, Category="Aura")
	TObjectPtr<USphereComponent> SphereComp;
	
	/* Function */
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
