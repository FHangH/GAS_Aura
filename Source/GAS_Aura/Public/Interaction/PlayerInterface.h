// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API IPlayerInterface
{
	GENERATED_BODY()

	/* Function */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void AddToXP(const int32 InXP);
};
