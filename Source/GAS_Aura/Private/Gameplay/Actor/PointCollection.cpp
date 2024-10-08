// Copyright fangh.space


#include "Gameplay/Actor/PointCollection.h"
#include "Kismet/KismetMathLibrary.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"

APointCollection::APointCollection()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneComponent_Pt_0 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt0"));
	SetRootComponent(SceneComponent_Pt_0);
	ImmutablePts.Add(SceneComponent_Pt_0);
	
	SceneComponent_Pt_1 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt1"));
	SceneComponent_Pt_1->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_1);
	
	SceneComponent_Pt_2 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt2"));
	SceneComponent_Pt_2->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_2);
	
	SceneComponent_Pt_3 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt3"));
	SceneComponent_Pt_3->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_3);
	
	SceneComponent_Pt_4 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt4"));
	SceneComponent_Pt_4->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_4);
	
	SceneComponent_Pt_5 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt5"));
	SceneComponent_Pt_5->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_5);
	
	SceneComponent_Pt_6 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt6"));
	SceneComponent_Pt_6->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_6);
	
	SceneComponent_Pt_7 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt7"));
	SceneComponent_Pt_7->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_7);
	
	SceneComponent_Pt_8 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt8"));
	SceneComponent_Pt_8->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_8);
	
	SceneComponent_Pt_9 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt9"));
	SceneComponent_Pt_9->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_9);
	
	SceneComponent_Pt_10 = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent_Pt10"));
	SceneComponent_Pt_10->SetupAttachment(RootComponent);
	ImmutablePts.Add(SceneComponent_Pt_10);
	
	SceneComponent_Pt_1->SetWorldLocation(FVector(250.f, 0.f, 0.f));
	SceneComponent_Pt_2->SetWorldLocation(FVector(202.25f, 146.95f, 0.f));
	SceneComponent_Pt_3->SetWorldLocation(FVector(77.25f, 237.76f, 0.f));
	SceneComponent_Pt_4->SetWorldLocation(FVector(-77.25f, 237.76f, 0.f));
	SceneComponent_Pt_5->SetWorldLocation(FVector(-202.25f, 146.95f, 0.f));
	SceneComponent_Pt_6->SetWorldLocation(FVector(-250.f, 0.f, 0.f));
	SceneComponent_Pt_7->SetWorldLocation(FVector(-202.25f, -146.95f, 0.f));
	SceneComponent_Pt_8->SetWorldLocation(FVector(-77.25f, -237.76f, 0.f));
	SceneComponent_Pt_9->SetWorldLocation(FVector(77.25f, -237.76f, 0.f));
	SceneComponent_Pt_10->SetWorldLocation(FVector(202.25f, -146.95f, 0.f));
}

void APointCollection::BeginPlay()
{
	Super::BeginPlay();
}

TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, const int32 NumPoints, const float YawOverride)
{
	checkf(ImmutablePts.Num() >= NumPoints, TEXT("NumPoints is too large for the number of points available"));

	TArray<USceneComponent*> ArrayCopy;
	for (const auto& Pt : ImmutablePts)
	{
		if (ArrayCopy.Num() >= NumPoints) return ArrayCopy;

		if (Pt != SceneComponent_Pt_0)
		{
			auto ToPoint = Pt->GetComponentLocation() - SceneComponent_Pt_0->GetComponentLocation();
			ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
			Pt->SetWorldLocation(SceneComponent_Pt_0->GetComponentLocation() + ToPoint);
		}

		auto RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
		auto LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
		
		TArray<AActor*> OutActorsToIgnore;
		UAuraAbilitySystemFuncLibrary::GetLivePlayersWithRadius(
			this, OutActorsToIgnore, TArray<AActor*>{}, 1500.f, GetActorLocation());

		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActors(OutActorsToIgnore);
		GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams);

		const auto AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
		Pt->SetWorldLocation(AdjustedLocation);
		Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

		ArrayCopy.Add(Pt);
	}
	return ArrayCopy;
}

/*TArray<USceneComponent*> APointCollection::GetGroundPoints(const FVector& GroundLocation, const int32 NumPoints, const float YawOverride)
{
    checkf(ImmutablePts.Num() >= NumPoints, TEXT("NumPoints is too large for the number of points available"));

    TArray<USceneComponent*> ArrayCopy;

    if (!SceneComponent_Pt_0)
    {
        UE_LOG(LogTemp, Error, TEXT("SceneComponent_Pt0 is null!"));
        return ArrayCopy; // 提前返回
    }

    for (const auto& Pt : ImmutablePts)
    {
        if (!Pt || ArrayCopy.Num() >= NumPoints)
        {
        	UE_LOG(LogTemp, Error, TEXT("PT is null!"));	
        	continue; // 跳过空指针或达到所需数量
        }

        auto ToPoint = Pt->GetComponentLocation() - SceneComponent_Pt_0->GetComponentLocation();
        ToPoint = ToPoint.RotateAngleAxis(YawOverride, FVector::UpVector);
        Pt->SetWorldLocation(SceneComponent_Pt_0->GetComponentLocation() + ToPoint);

        auto RaisedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z + 500.f);
        auto LoweredLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, Pt->GetComponentLocation().Z - 500.f);
        
        TArray<AActor*> OutActorsToIgnore;
        UAuraAbilitySystemFuncLibrary::GetLivePlayersWithRadius(
        	this, OutActorsToIgnore, TArray<AActor*>{}, 1500.f, GetActorLocation());

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActors(OutActorsToIgnore);

        if (bool bHit = GetWorld()->LineTraceSingleByProfile(HitResult, RaisedLocation, LoweredLocation, FName("BlockAll"), QueryParams))
        {
            const auto AdjustedLocation = FVector(Pt->GetComponentLocation().X, Pt->GetComponentLocation().Y, HitResult.ImpactPoint.Z);
            Pt->SetWorldLocation(AdjustedLocation);
            Pt->SetWorldRotation(UKismetMathLibrary::MakeRotFromZ(HitResult.ImpactNormal));

            ArrayCopy.Add(Pt);
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Line trace did not hit anything."));
        }
    }
    return ArrayCopy;
}*/
