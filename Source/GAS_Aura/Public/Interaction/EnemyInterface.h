// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnemyInterface.generated.h"

UINTERFACE(MinimalAPI, BlueprintType, Category="Aura|Interface")
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API IEnemyInterface
{
	GENERATED_BODY()

	/* Function */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Enemy")
	void SetCombatTarget(AActor* InCombatTarget);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Enemy")
	AActor* GetCombatTarget() const;
};
