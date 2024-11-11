// Copyright fangh.space


#include "Gameplay/Actor/CheckPoint/MapEntrance.h"
#include "Components/SphereComponent.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Interaction/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	if (SphereComponent)
	{
		SphereComponent->SetupAttachment(MoveToSceneComponent);
	}
}

void AMapEntrance::OnSphereStartOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->Implements<UPlayerInterface>())
	{
		IsReached = true;

		if (const auto AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			AuraGM->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}
		IPlayerInterface::Execute_SaveProgress(OtherActor, PlayerStartTag);
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}

// Save Interface
void AMapEntrance::LoadActor_Implementation()
{
	Super::LoadActor_Implementation();
}

// HighLight Interface
void AMapEntrance::HighLightActor_Implementation()
{
	if (CheckPointMeshComponent)
	{
		CheckPointMeshComponent->SetRenderCustomDepth(true);
	}
}
