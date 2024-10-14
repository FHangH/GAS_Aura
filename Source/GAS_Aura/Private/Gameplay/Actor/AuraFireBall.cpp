// Copyright fangh.space


#include "Gameplay/Actor/AuraFireBall.h"

AAuraFireBall::AAuraFireBall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraFireBall::StartOutgoingTimeLine_Implementation()
{
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();

	StartOutgoingTimeLine();
}

void AAuraFireBall::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}