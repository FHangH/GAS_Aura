// Copyright fangh.space


#include "Input/AuraInputComponent.h"

UAuraInputComponent::UAuraInputComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAuraInputComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAuraInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}