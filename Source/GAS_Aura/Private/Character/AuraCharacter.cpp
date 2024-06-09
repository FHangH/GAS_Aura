// Copyright fangh.space


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/Data/DataAsset_LevelUpInfo.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "Untils/AuraLog.h"

AAuraCharacter::AAuraCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = {0.f, 480.f, 0.f};
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LevelUpNiagaraComponent"));
	LevelUpNiagaraComponent->SetupAttachment(RootComponent);
	LevelUpNiagaraComponent->bAutoActivate = false;

	TopDownSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("TopDownSpringArm"));
	TopDownSpringArm->SetupAttachment(RootComponent);
	TopDownSpringArm->SetUsingAbsoluteRotation(true);
	TopDownSpringArm->bDoCollisionTest = false;
	
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCameraComponent"));
	TopDownCameraComponent->SetupAttachment(TopDownSpringArm, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

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

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const auto AuraPS = GetPlayerState<AAuraPlayerState>();
	return AuraPS ? AuraPS->GetPlayerLevel() : 0;
}

int32 AAuraCharacter::FindLevelForXP_Implementation(const int32 InXP) const
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPS->DA_LevelUpInfo->FindLevelForXP(InXP);
	}
	return 0;
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPS->GetPlayerXP();
	}
	return 0;
}

int32 AAuraCharacter::GetAttributePointReward_Implementation(const int32 Level) const
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPS->DA_LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPointReward_Implementation(const int32 Level) const
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		return AuraPS->DA_LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
	}
	return 0;
}

void AAuraCharacter::AddToXP_Implementation(const int32 InXP)
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToXP(InXP);
	}
}

void AAuraCharacter::AddToPlayerLevel_Implementation(const int32 InPlayerLevel)
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddLevel(InPlayerLevel);
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToAttributePoints(InAttributePoints);
	}
}

void AAuraCharacter::AddToSpellPoints_Implementation(const int32 InSpellPoints)
{
	if (const auto AuraPS = GetPlayerState<AAuraPlayerState>())
	{
		AuraPS->AddToSpellPoints(InSpellPoints);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	MultiCast_LevelUpParticle();
}

void AAuraCharacter::MultiCast_LevelUpParticle_Implementation()
{
	if (LevelUpNiagaraComponent->GetAsset())
	{
		const auto CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const auto NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const auto ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
		return;
	}
	UE_LOG(Aura, Warning, TEXT("LevelUpNiagaraComponent System Asset is not set"));
}
