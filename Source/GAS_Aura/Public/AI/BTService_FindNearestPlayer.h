// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlueprintBase.h"
#include "BTService_FindNearestPlayer.generated.h"

UCLASS()
class GAS_AURA_API UBTService_FindNearestPlayer : public UBTService_BlueprintBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Aura")
	FBlackboardKeySelector TargetToFollowSelector;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Aura")
	FBlackboardKeySelector DistanceToTargetSelector;
	
	/* Function */
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
