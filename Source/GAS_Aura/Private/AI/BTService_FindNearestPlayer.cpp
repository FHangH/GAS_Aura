// Copyright fangh.space


#include "AI/BTService_FindNearestPlayer.h"
#include "AIController.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void UBTService_FindNearestPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!AIOwner) return;

	OwingPawn = AIOwner->GetPawn();
	const auto TargetTag = OwingPawn->ActorHasTag(FName("Player")) ? FName("Enemy") : FName("Player");
	auto ClosestDistance { TNumericLimits<float>::Max() };
	
	UGameplayStatics::GetAllActorsWithTag(OwingPawn, TargetTag, ActorsWithTag);
	for (const auto& Actor : ActorsWithTag)
	{
		if (!Actor || !OwingPawn) continue;
		if (const auto Distance = OwingPawn->GetDistanceTo(Actor); Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	UBTFunctionLibrary::SetBlackboardValueAsObject(this, TargetToFollowSelector, ClosestActor);
	UBTFunctionLibrary::SetBlackboardValueAsFloat(this, DistanceToTargetSelector, ClosestDistance);
}
