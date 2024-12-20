// Copyright fangh.space


#include "Character/AuraCharacter.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Gameplay/GameMode/AuraGameModeBase.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_LevelUpInfo.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "Gameplay/SaveGame/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"
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

	BaseWalkSpeed = 600.f;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init Server
	InitAbilityActorInfo();
	LoadProgress();

	if (CHECK_GAME_MODE(AuraGameMode))
	{
		AuraGameMode->LoadWorldState(GetWorld());
	}
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init Client
	InitAbilityActorInfo();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
		Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();

		ASComponent = AuraPlayerState->GetAbilitySystemComponent();
		AS = AuraPlayerState->GetAttributeSet();
		OnASComponentRegisteredDelegate.Broadcast(ASComponent);
		ASComponent->RegisterGameplayTagEvent(
			FAuraGameplayTags::Get().DeBuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ThisClass::StunTagChanged);

		if (CHECK_PLAYER_CONTROLLER(AuraPlayerController))
		{
			const auto AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD());
			AuraHUD->InitOverlayMain(AuraPlayerController, AuraPlayerState, ASComponent, AS);
		}
	}
}

void AAuraCharacter::LoadProgress()
{
	if (!CHECK_GAME_MODE(AuraGameMode) || !CHECK_PLAYER_STATE(AuraPlayerState) || !CHECK_ABILITY_SYSTEM_COMPONENT(AuraASComponent)) return;
	if (const auto SaveData = AuraGameMode->RetrieveInGameSaveData())
	{
		if (SaveData->IsFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			AuraASComponent->AddCharacterAbilitiesFromSaveData(SaveData);
			AuraPlayerState->SetLevel(SaveData->PlayerLevel);
			AuraPlayerState->SetXP(SaveData->XP);
			AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
			AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
			
			UAuraAbilitySystemFuncLibrary::InitializeDefaultAttributesFromSaveData(this, AuraASComponent, SaveData);
		}
	}
}

// Combat Interface
int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	return CHECK_PLAYER_STATE(AuraPlayerState) ? AuraPlayerState->GetPlayerLevel() : 0;
}

// Player Interface
int32 AAuraCharacter::FindLevelForXP_Implementation(const int32 InXP)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->DA_LevelUpInfo->FindLevelForXP(InXP);
	}
	return 0;
}

int32 AAuraCharacter::GetXP_Implementation()
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->GetPlayerXP();
	}
	return 0;
}

int32 AAuraCharacter::GetAttributePointReward_Implementation(const int32 Level)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->DA_LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPointReward_Implementation(const int32 Level)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->DA_LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
	}
	return 0;
}

int32 AAuraCharacter::GetAttributePoints_Implementation()
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->GetAttributePoints();
	}
	return 0;
}

int32 AAuraCharacter::GetSpellPoints_Implementation()
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		return AuraPlayerState->GetSpellPoints();
	}
	return 0;
}

void AAuraCharacter::AddToXP_Implementation(const int32 InXP)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		AuraPlayerState->AddToXP(InXP);
	}
}

void AAuraCharacter::AddToPlayerLevel_Implementation(const int32 InPlayerLevel)
{
	if (!CHECK_PLAYER_STATE(AuraPlayerState) || !CHECK_ABILITY_SYSTEM_COMPONENT(AuraASComponent)) return;
	AuraPlayerState->AddLevel(InPlayerLevel);
	AuraASComponent->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
}

void AAuraCharacter::AddToAttributePoints_Implementation(const int32 InAttributePoints)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		AuraPlayerState->AddToAttributePoints(InAttributePoints);
	}
}

void AAuraCharacter::AddToSpellPoints_Implementation(const int32 InSpellPoints)
{
	if (CHECK_PLAYER_STATE(AuraPlayerState))
	{
		AuraPlayerState->AddToSpellPoints(InSpellPoints);
	}
}

void AAuraCharacter::LevelUp_Implementation()
{
	MultiCast_LevelUpParticle();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (CHECK_PLAYER_CONTROLLER(AuraPlayerController))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (CHECK_PLAYER_CONTROLLER(AuraPlayerController))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckPointTag)
{
	if (!CHECK_GAME_MODE(AuraGameMode) || !CHECK_PLAYER_STATE(AuraPlayerState) || !CHECK_ABILITY_SYSTEM_COMPONENT(AuraASComponent)) return;
	if (const auto SaveData = AuraGameMode->RetrieveInGameSaveData())
	{
		SaveData->PlayerStartTag = CheckPointTag;
		SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
		SaveData->XP = AuraPlayerState->GetPlayerXP();
		SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
		SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		SaveData->IsFirstTimeLoadIn = false;

		if (!HasAuthority()) return;
		FForEachAbilitySignature SaveAbilityDelegate;
		SaveData->SavedAbilities.Empty();
		
		SaveAbilityDelegate.BindLambda([this, &SaveData](const FGameplayAbilitySpec& AbilitySpec)
		{
			const auto AbilityTag = AuraASComponent->GetAbilityTagFromSpec(AbilitySpec);
			const auto AbilityInfo = UAuraAbilitySystemFuncLibrary::GetAbilityInfo(this);
			const auto Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

			FSavedAbility SavedAbility;
			SavedAbility.GameplayAbilityClass = Info.Ability;
			SavedAbility.AbilityLevel = AbilitySpec.Level;
			SavedAbility.AbilitySlot = AuraASComponent->GetSlotFromAbilityTag(AbilityTag);
			SavedAbility.AbilityStatus = AuraASComponent->GetStatusFromAbilityTag(AbilityTag);
			SavedAbility.AbilityTag = AbilityTag;
			SavedAbility.AbilityType = Info.AbilityType;
			
			SaveData->SavedAbilities.AddUnique(SavedAbility);
		});
		AuraASComponent->ForEachAbility(SaveAbilityDelegate);
		AuraGameMode->SaveInGameProgressData(SaveData);
	}
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

void AAuraCharacter::OnRep_IsStunned()
{
	if (CHECK_ABILITY_SYSTEM_COMPONENT(AuraASComponent))
	{
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(FAuraGameplayTags::Get().Player_Block_CursorTrace);
		BlockedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputPressed);
		BlockedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputHeld);
		BlockedTags.AddTag(FAuraGameplayTags::Get().Player_Block_InputReleased);

		if (bIsStunned)
		{
			AuraASComponent->AddLooseGameplayTags(BlockedTags);
			if (NiagaraComponent_DeBuff_Stun) NiagaraComponent_DeBuff_Stun->Activate();
		}
		else
		{
			AuraASComponent->RemoveLooseGameplayTags(BlockedTags);
			if (NiagaraComponent_DeBuff_Stun) NiagaraComponent_DeBuff_Stun->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_IsBurned()
{
	if (!NiagaraComponent_DeBuff_Burn) return;
	if (bIsBurned)
	{
		NiagaraComponent_DeBuff_Burn->Activate();
	}
	else
	{
		NiagaraComponent_DeBuff_Burn->Deactivate();
	}
}
