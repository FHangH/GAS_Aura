// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HighLightInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Category="Aura|Interface")
class UHighLightInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API IHighLightInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|HighLight")
	void HighLightActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|HighLight")
	void UnHighLightActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|HighLight")
	void SetMoveToLocation(FVector& Location);
};
