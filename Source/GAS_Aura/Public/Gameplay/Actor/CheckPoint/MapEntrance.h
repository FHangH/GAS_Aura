// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "CheckPoint.h"
#include "MapEntrance.generated.h"

UCLASS()
class GAS_AURA_API AMapEntrance : public ACheckPoint
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|MapEntrance", meta=(AllowPrivateAccess=true))
	TSoftObjectPtr<UWorld> DestinationMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|MapEntrance", meta=(AllowPrivateAccess=true))
	FName DestinationPlayerStartTag;
	
	/* Function */
public:
	AMapEntrance(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	// Save Interface
	virtual void LoadActor_Implementation() override;
	
	// HighLight Interface
	virtual void HighLightActor_Implementation() override;
};
