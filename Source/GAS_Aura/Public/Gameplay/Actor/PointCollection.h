// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PointCollection.generated.h"

UCLASS()
class GAS_AURA_API APointCollection : public AActor
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true), meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_1;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_2;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_3;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_4;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_5;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_6;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_7;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_8;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_9;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TObjectPtr<USceneComponent> SceneComponent_Pt_10;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Aura", meta=(AllowPrivateAccess=true))
	TArray<USceneComponent*> ImmutablePts;

	/* Function */
public:
	APointCollection();

protected:
	virtual void BeginPlay() override;

	// Self
public:
	UFUNCTION(BlueprintPure, Category="Aura", meta=(AllowPrivateAccess=true))
	TArray<USceneComponent*> GetGroundPoints(const FVector& GroundLocation, const int32 NumPoints, const float YawOverride = 0.f);
};
