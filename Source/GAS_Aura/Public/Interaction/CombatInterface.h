// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual int32 GetPlayerLevel();
	virtual FVector GetCombatSocketLocation();
};
