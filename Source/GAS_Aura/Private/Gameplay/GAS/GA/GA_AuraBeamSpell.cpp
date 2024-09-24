// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraBeamSpell.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"

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

		if (HitResult.bBlockingHit)
		{
			MouseHitLocation = HitResult.ImpactPoint;
			MouseHitActor = HitResult.GetActor();
		}
	}

	if (!MouseHitActor) return;
	if (const auto CombatInterface = Cast<ICombatInterface>(MouseHitActor))
	{
		if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::PrimaryTargetDied)) return;
		CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::PrimaryTargetDied);
	}
}

void UGA_AuraBeamSpell::StoreAdditionalTargets(TArray<AActor*>& OutAdditionalTargets) const
{
	TArray<AActor*> OverlappingActors;
	TArray<AActor*> IgnoreActors;
	IgnoreActors.AddUnique(GetAvatarActorFromActorInfo());
	IgnoreActors.AddUnique(MouseHitActor);

	UAuraAbilitySystemFuncLibrary::GetLivePlayersWithRadius(
		GetAvatarActorFromActorInfo(),
		OverlappingActors,
		IgnoreActors,
		850.f,
		MouseHitActor->GetActorLocation());

	const auto NumAdditionalTargets = FMath::Min(GetAbilityLevel() - 1, MaxNumShockTargets);
	//const int32 NumAdditionalTargets = MaxNumShockTargets;
	
	UAuraAbilitySystemFuncLibrary::GetClosestTargets(
		NumAdditionalTargets,
		OverlappingActors,
		MouseHitActor->GetActorLocation(),
		OutAdditionalTargets);

	for (const auto& Target : OutAdditionalTargets)
	{
		if (const auto CombatInterface = Cast<ICombatInterface>(Target))
		{
			if (CombatInterface->GetOnDeathDelegate().IsAlreadyBound(this, &ThisClass::AdditionalTargetDied)) return;
			CombatInterface->GetOnDeathDelegate().AddDynamic(this, &ThisClass::AdditionalTargetDied);
		}
	}
}

void UGA_AuraBeamSpell::PrimaryTargetDied_Implementation(AActor* DeadActor)
{
}

void UGA_AuraBeamSpell::AdditionalTargetDied_Implementation(AActor* DeadActor)
{
}