// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AuraAIController.generated.h"

class UBehaviorTreeComponent;

UCLASS()
class GAS_AURA_API AAuraAIController : public AAIController
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UBehaviorTreeComponent> BeBehaviorTreeComponent;

	/* Function */
public:
	AAuraAIController();

protected:
	virtual void BeginPlay() override;
};
