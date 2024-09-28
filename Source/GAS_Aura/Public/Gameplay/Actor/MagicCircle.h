// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

UCLASS()
class GAS_AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura|DecalComponent")
	TObjectPtr<UDecalComponent> Decal_MagicCircle;
	
	/* Function */
	AMagicCircle();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
