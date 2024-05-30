// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraSummon.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Untils/AuraLog.h"

TArray<FVector> UGA_AuraSummon::GetSpawnLocations()
{
	if (MinionClasses.IsEmpty() || MinionClasses[0] == nullptr)
	{
		UE_LOG(Aura, Warning, TEXT("MinionClasses Array is empty"));
	}
	if (!GetAvatarActorFromActorInfo()) return TArray<FVector>();

	const auto Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const auto Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const auto DeltaSpread = SpawnSpread / NumMinions;
	TArray<FVector> SpawnLocations;

	// Calculate LeftSpread To RightSpread Spawn Location
	const auto LeftOfSpread = Forward.RotateAngleAxis(-SpawnSpread / 2.f, FVector::UpVector);
	for (int32 i = 0; i < NumMinions; ++i)
	{
		const auto Direction = LeftOfSpread.RotateAngleAxis(i * DeltaSpread, FVector::UpVector);
		auto ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);

		// Check if the spawn location is valid
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(
			HitResult,
			ChosenSpawnLocation + FVector{0.f, 0.f, 400.f},
			ChosenSpawnLocation - FVector{0.f, 0.f, 400.f},
			ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			ChosenSpawnLocation = HitResult.ImpactPoint;
		}
		SpawnLocations.Add(ChosenSpawnLocation);

		// DrawDebug
		DebugSpawnLocations(ChosenSpawnLocation, Location, Direction);
	}
	
	return SpawnLocations;
}

TSubclassOf<APawn> UGA_AuraSummon::GetRandomMinionClass()
{
	if (MinionClasses.IsEmpty() || MinionClasses[0] == nullptr)
	{
		UE_LOG(Aura, Warning, TEXT("MinionClasses Array is empty"));
		return nullptr;
	}
    return MinionClasses[FMath::RandRange(0, MinionClasses.Num() - 1)];
}

void UGA_AuraSummon::DebugSpawnLocations(const FVector& SpawnLocation, const FVector& StartLocation, const FVector& SpawnDirection) const
{
	if (!bDebugSpawnLocations) return;
	
#if UE_EDITOR
	// Debug Really Spawn Location
	DrawDebugSphere(GetWorld(), SpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);

	// Debug Spawn Location Direction
	UKismetSystemLibrary::DrawDebugArrow(
		GetAvatarActorFromActorInfo(),
		StartLocation,
		StartLocation + SpawnDirection * MaxSpawnDistance,
		4.f,
		FLinearColor::Green,
		3.f);

	// Debug Spawn Location Range
	DrawDebugSphere(GetWorld(), StartLocation + SpawnDirection * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
	DrawDebugSphere(GetWorld(), StartLocation + SpawnDirection * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
#endif
}
