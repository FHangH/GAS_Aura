// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CursorTraceInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCursorTraceInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API ICursorTraceInterface
{
	GENERATED_BODY()

	/* Function */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|CursorTrace")
	void SetCursorTraceMode(bool bEnable);
};
