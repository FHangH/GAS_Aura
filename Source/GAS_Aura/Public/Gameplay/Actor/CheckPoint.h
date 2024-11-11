// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "Interaction/HighLightInterface.h"
#include "Interaction/SaveInterface.h"
#include "Untils/RenderDepth.h"
#include "CheckPoint.generated.h"

class USphereComponent;

UCLASS()
class GAS_AURA_API ACheckPoint : public APlayerStart, public ISaveInterface, public IHighLightInterface
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Aura|CheckPoint", meta=(AllowPrivateAccess=true))
	TObjectPtr<UStaticMeshComponent> CheckPointMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Aura|CheckPoint", meta=(AllowPrivateAccess=true))
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="Aura|CheckPoint", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> MoveToSceneComponent;

public:
	UPROPERTY(BlueprintReadWrite, SaveGame, Category="Aura|CheckPoint")
	bool IsReached { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|CheckPoint")
	int32 CustomRenderDepth { RENDER_DEPTH_TAN };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|CheckPoint")
	bool IsBindOverlapCallBack { true };

	/* Function */
public:
	explicit ACheckPoint(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	// Save Interface
	virtual bool ShouldLoadTransform_Implementation() override;
	virtual void LoadActor_Implementation() override;

	// HighLight Interface
	virtual void HighLightActor_Implementation() override;
	virtual void UnHighLightActor_Implementation() override;
	virtual void SetMoveToLocation_Implementation(FVector& Location) override;

protected:
	UFUNCTION()
	virtual void OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintNativeEvent, Category="Aura|CheckPoint")
	void CheckPointReached(UMaterialInstanceDynamic* MaterialInstanceDynamic);

	UFUNCTION(BlueprintCallable, Category="Aura|CheckPoint")
	void HandleGlowEffects();
};
