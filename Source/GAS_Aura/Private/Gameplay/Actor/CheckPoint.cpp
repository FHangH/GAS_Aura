﻿// Copyright fangh.space


#include "Gameplay/Actor/CheckPoint.h"
#include "Components/SphereComponent.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

ACheckPoint::ACheckPoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	CheckPointMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CheckPointMeshComponent"));
	CheckPointMeshComponent->SetupAttachment(RootComponent);
	CheckPointMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CheckPointMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(CheckPointMeshComponent);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

bool ACheckPoint::ShouldLoadTransform_Implementation()
{
	return false;
}

void ACheckPoint::LoadActor_Implementation()
{
	if (IsReached)
	{
		HandleGlowEffects();
	}
}

void ACheckPoint::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ACheckPoint::OnSphereStartOverlap);
}

void ACheckPoint::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		IsReached = true;

		if (const auto AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		HandleGlowEffects();
	}
}

void ACheckPoint::CheckPointReached_Implementation(UMaterialInstanceDynamic* MaterialInstanceDynamic)
{
}

void ACheckPoint::HandleGlowEffects()
{
	if (!SphereComponent || !CheckPointMeshComponent) return;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);

	if (const auto DynamicMatInstance = UMaterialInstanceDynamic::Create(CheckPointMeshComponent->GetMaterial(0), this))
	{
		CheckPointMeshComponent->SetMaterial(0, DynamicMatInstance);
		CheckPointReached(DynamicMatInstance);
	}
}