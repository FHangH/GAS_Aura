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
		const auto ChosenSpawnLocation = Location + Direction * FMath::FRandRange(MinSpawnDistance, MaxSpawnDistance);
		SpawnLocations.Add(ChosenSpawnLocation);
		
		// Debug Really Spawn Location
		DrawDebugSphere(GetWorld(), ChosenSpawnLocation, 18.f, 12, FColor::Cyan, false, 3.f);

		// Debug Spawn Location Direction
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			Location,
			Location + Direction * MaxSpawnDistance,
			4.f,
			FLinearColor::Green,
			3.f);

		// Debug Spawn Location Range
		DrawDebugSphere(GetWorld(), Location + Direction * MinSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
		DrawDebugSphere(GetWorld(), Location + Direction * MaxSpawnDistance, 5.f, 12, FColor::Red, false, 3.f);
	}
	
	return SpawnLocations;
}
