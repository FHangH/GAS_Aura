﻿// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Category="Aura|Interface")
class USaveInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API ISaveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Save")
	bool ShouldLoadTransform();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interface|Save")
	void LoadActor();
};
