// Copyright fangh.space


#include "Gameplay/Actor/MagicCircle.h"
#include "Components/DecalComponent.h"

AMagicCircle::AMagicCircle()
{
	PrimaryActorTick.bCanEverTick = true;

	Decal_MagicCircle = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp_MagicCircle"));
	Decal_MagicCircle->SetupAttachment(RootComponent);
}

void AMagicCircle::BeginPlay()
{
	Super::BeginPlay();
}

void AMagicCircle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}