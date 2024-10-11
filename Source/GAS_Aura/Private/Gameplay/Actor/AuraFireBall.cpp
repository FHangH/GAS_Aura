// Copyright fangh.space


#include "Gameplay/Actor/AuraFireBall.h"

AAuraFireBall::AAuraFireBall()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAuraFireBall::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraFireBall::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}