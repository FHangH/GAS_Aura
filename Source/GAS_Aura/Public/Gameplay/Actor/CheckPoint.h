// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/SaveInterface.h"
#include "CheckPoint.generated.h"

class USphereComponent;

UCLASS()
class GAS_AURA_API ACheckPoint : public APlayerStart, public ISaveInterface
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleAnywhere, Category="Aura|CheckPoint")
	TObjectPtr<UStaticMeshComponent> CheckPointMeshComponent;

	UPROPERTY(VisibleAnywhere, Category="Aura|CheckPoint")
	TObjectPtr<USphereComponent> SphereComponent;

public:
	UPROPERTY(BlueprintReadOnly, SaveGame, Category="Aura|CheckPoint")
	bool IsReached { false };

	/* Function */
public:
	explicit ACheckPoint(const FObjectInitializer& ObjectInitializer);

	// Save Interface
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category="Aura|CheckPoint")
	void CheckPointReached(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	UFUNCTION()
	void HandleGlowEffects();
};
