// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void UGA_AuraBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UGA_AuraBeamSpell::StoreOwners()
{
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor.Get());
	}
}

void UGA_AuraBeamSpell::TraceFirstTarget(const FVector& BeamTargetLocation)
{
	if (!OwnerCharacter || !OwnerCharacter->Implements<UCombatInterface>()) return;

	if (const auto Weapon = ICombatInterface::Execute_GetWeaponMesh(OwnerCharacter))
	{
		const auto SocketLocation = Weapon->GetSocketLocation(FName{"TipSocket"});
		
		TArray<AActor*> IgnoresActor;
		IgnoresActor.AddUnique(OwnerCharacter);

		FHitResult HitResult;
		
		UKismetSystemLibrary::SphereTraceSingle(
			OwnerCharacter,
			SocketLocation,
			BeamTargetLocation,
			10.f,
			TraceTypeQuery1,
			false, 
			IgnoresActor,
			EDrawDebugTrace::None,
			HitResult,
			true);

		if (HitResult.IsValidBlockingHit())
		{
			MouseHitLocation = HitResult.ImpactPoint;
			MouseHitActor = HitResult.GetActor();
		}
	}
}
