// Copyright fangh.space


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = {0.f, 480.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClassType = ECharacterClassType::ECT_Elementalist;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Client
	InitAbilityActorInfo();
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const auto AuraPS = GetPlayerState<AAuraPlayerState>();
	return AuraPS ? AuraPS->GetPlayerLevel() : 0;
}

void AAuraCharacter::AddToXP_Implementation(const int32 InXP)
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToXP(InXP);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	if (const auto AuraPlayerState = GetPlayerState<AAuraPlayerState>())
	{
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
		Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

		ASComponent = AuraPlayerState->GetAbilitySystemComponent();
		AS = AuraPlayerState->GetAttributeSet();

		if (const auto AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
		{
			const auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD());
			AuraHUD->InitOverlayMain(AuraPlayerController, AuraPlayerState, ASComponent, AS);
		}

		InitializeDefaultAttributes();
	}
}
