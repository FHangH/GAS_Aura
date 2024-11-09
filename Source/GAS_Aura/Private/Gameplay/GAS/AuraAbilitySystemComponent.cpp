// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/GAS/AuraGameplayAbility.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/SaveGame/LoadScreenSaveGame.h"
#include "Interaction/PlayerInterface.h"
#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if (!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		OnAbilityGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::Client_OnEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpAbilities)
{
	for (const auto& GAClass : StartUpAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{GAClass, 1};
		
		if (const auto AuraGA = Cast<UAuraGameplayAbility>(GASpec.Ability))
		{
			GASpec.DynamicAbilityTags.AddTag(AuraGA->StartUpInputTag);
			GASpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Ability_Status_Equipped);
			GiveAbility(GASpec);
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartUpPassiveAbilities)
{
	for (const auto& GAClass : StartUpPassiveAbilities)
	{
		auto GASpec = FGameplayAbilitySpec{GAClass, 1};
		GASpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Ability_Status_Equipped);
		GiveAbilityAndActivateOnce(GASpec);
	}
}

void UAuraAbilitySystemComponent::AddCharacterAbilitiesFromSaveData(ULoadScreenSaveGame* SaveGame)
{
	for (const auto& Data : SaveGame->SavedAbilities)
	{
		const auto LoadedAbilityClass = Data.GameplayAbilityClass;
		auto LoadedAbilitySpec = FGameplayAbilitySpec{ LoadedAbilityClass, Data.AbilityLevel };

		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilitySlot);
		LoadedAbilitySpec.DynamicAbilityTags.AddTag(Data.AbilityStatus);
		
		if (Data.AbilityType == FAuraGameplayTags::Get().Ability_Type_Offensive)
		{
			GiveAbility(LoadedAbilitySpec);
		}
		else if (Data.AbilityType == FAuraGameplayTags::Get().Ability_Type_Passive)
		{
			if (Data.AbilityStatus.MatchesTagExact(FAuraGameplayTags::Get().Ability_Status_Equipped))
			{
				GiveAbilityAndActivateOnce(LoadedAbilitySpec);
			}
			else
			{
				GiveAbility(LoadedAbilitySpec);
			}
		}
	}
	bStartupAbilitiesGiven = true;
	OnAbilityGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AbilityInputPressed(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		AbilitySpecInputPressed(GASpec);
		if (GASpec.IsActive())
		{
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, GASpec.Handle, GASpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;

		AbilitySpecInputPressed(GASpec);
		if (!GASpec.IsActive())
		{
			TryActivateAbility(GASpec.Handle);
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

	FScopedAbilityListLock AbilityListLock(*this);
	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag) || !GASpec.IsActive()) continue;
		
		AbilitySpecInputReleased(GASpec);
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, GASpec.Handle, GASpec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbilitySignature& Delegate)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(Aura, Error, TEXT("Failed to execute delegate for ability spec: %hs"), __FUNCTION__);
		}
	}
}

bool UAuraAbilitySystemComponent::IsPassiveAbility(const FGameplayAbilitySpec& AbilitySpec) const
{
	const auto AbilityInfo = UAuraAbilitySystemFuncLibrary::GetAbilityInfo(GetAvatarActor());
	if (!AbilityInfo) return false;

	const auto AbilityTag = GetAbilityTagFromSpec(AbilitySpec);
	const auto Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	const auto AbilityType = Info.AbilityType;
	return AbilityType.MatchesTagExact(FAuraGameplayTags::Get().Ability_Type_Passive);
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if (AbilitySpec.Ability)
	{
		for (const auto& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
			{
				return Tag;
			}
		}
	}
	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const auto& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
		{
			return Tag;
		}
	}
	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for (const auto& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability.Status"))))
		{
			return Tag;
		}
	}
	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const auto Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusTagFromSpec(*Spec);
	}
	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetSlotFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if (const auto Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagFromSpec(*Spec);
	}
	return FGameplayTag{};
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopedLock(*this);
	
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		for (const auto& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if (Tag.MatchesTag(AbilityTag)) return &AbilitySpec;
		}
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (GetAvatarActor() && GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if (IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			Server_UpgradeAttribute(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(const int32 Level)
{
	if (!GetAvatarActor()) return;
	const auto DA_AbilityInfo = UAuraAbilitySystemFuncLibrary::GetAbilityInfo(GetAvatarActor());

	for (const auto& Info : DA_AbilityInfo->AbilityInformation)
	{
		if (!Info.AbilityTag.IsValid()) continue;
		if (Level < Info.LevelRequirement) continue;
		if (GetSpecFromAbilityTag(Info.AbilityTag) == nullptr)
		{
			auto AbilitySpec = FGameplayAbilitySpec{Info.Ability, 1};
			AbilitySpec.DynamicAbilityTags.AddTag(FAuraGameplayTags::Get().Ability_Status_Eligible);
			
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			Client_UpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Ability_Status_Eligible, 1);
		}
	}
}

bool UAuraAbilitySystemComponent::GetDescriptionFromAbilityTag(
	const FGameplayTag& AbilityTag, FString& OutDescription, FString& OutNextLevelDescription)
{
	if (const auto AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if (const auto AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetDescription(AbilitySpec->Level + 1);
			return true;
		}
	}
	const auto AbilityInfo = UAuraAbilitySystemFuncLibrary::GetAbilityInfo(GetAvatarActor());

	if (!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(FAuraGameplayTags::Get().Ability_None))
	{
		OutDescription = {};
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoForTag(AbilityTag).LevelRequirement);
	}
	OutNextLevelDescription = {};
	return false;
}

// Slot
bool UAuraAbilitySystemComponent::SlotIsEmpty(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock AbilityListLock(*this);

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&AbilitySpec, SlotTag)) return false;
	}
	return true;
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(const FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag)
{
	return Spec->DynamicAbilityTags.HasTagExact(SlotTag);
}

bool UAuraAbilitySystemComponent::AbilityHasAnySlot(const FGameplayAbilitySpec* Spec)
{
	return Spec->DynamicAbilityTags.HasTag(FGameplayTag::RequestGameplayTag(FName{"InputTag"}));
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecWithSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock AbilityListLock(*this);

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.DynamicAbilityTags.HasTagExact(SlotTag)) return &AbilitySpec;
	}
	return nullptr;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const auto Slot = GetInputTagFromSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& SlotTag)
{
	FScopedAbilityListLock ActiveScopeLock(*this);

	for (auto& Spec : GetActivatableAbilities())
	{
		if (AbilityHasSlot(&Spec, SlotTag))
		{
			ClearSlot(&Spec);
		}
	}
}

void UAuraAbilitySystemComponent::AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& SlotTag)
{
	ClearSlot(&Spec);
	Spec.DynamicAbilityTags.AddTag(SlotTag);
}

void UAuraAbilitySystemComponent::Server_EquipAbility_Implementation(const FGameplayTag& AbilityTag, const FGameplayTag& SlotTag)
{
	if (const auto& AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		const FGameplayTag& PrevSlot = GetInputTagFromSpec(*AbilitySpec);
		const FGameplayTag& Status = GetStatusTagFromSpec(*AbilitySpec);

		if (Status == GameplayTags.Ability_Status_Equipped || Status == GameplayTags.Ability_Status_Unlocked)
		{
			// Handle Activation / Deactivation For Passive Abilities
			if (!SlotIsEmpty(SlotTag))
			{
				const auto SpecWithSlot = GetSpecWithSlot(SlotTag);
				if (!SpecWithSlot) return;

				// Is That Ability the Same as this Ability ? If So, return early!
				if (AbilityTag.MatchesTagExact(GetAbilityTagFromSpec(*SpecWithSlot)))
				{
					Client_EquipAbility(AbilityTag, GameplayTags.Ability_Status_Equipped, SlotTag, PrevSlot);
					return;
				}
				if (IsPassiveAbility(*SpecWithSlot))
				{
					MultiCast_ActivatePassiveEffect(GetAbilityTagFromSpec(*SpecWithSlot), false);
					OnDeactivatePassiveAbilityDelegate.Broadcast(GetAbilityTagFromSpec(*SpecWithSlot));
				}
				ClearSlot(SpecWithSlot);
			}

			// Ability Doesn't Yet Have a Slot (It is not active)
			if (!AbilityHasAnySlot(AbilitySpec))
			{
				if (IsPassiveAbility(*AbilitySpec))
				{
					TryActivateAbility(AbilitySpec->Handle);
					MultiCast_ActivatePassiveEffect(AbilityTag, true);
				}
				AbilitySpec->DynamicAbilityTags.RemoveTag(GetStatusTagFromSpec(*AbilitySpec));
				AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_Status_Equipped);
			}
			AssignSlotToAbility(*AbilitySpec, SlotTag);
			MarkAbilitySpecDirty(*AbilitySpec);
		}
		Client_EquipAbility(AbilityTag, GameplayTags.Ability_Status_Equipped, SlotTag, PrevSlot);
	}
}

void UAuraAbilitySystemComponent::Client_EquipAbility_Implementation(
	const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot)
{
	OnAbilityEquippedDelegate.Broadcast(AbilityTag, StatusTag, SlotTag, PreviousSlot);
}

void UAuraAbilitySystemComponent::Client_OnEffectApplied_Implementation(
	UAbilitySystemComponent* ASComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveGEHandle) const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	OnEffectAssetTagDelegate.Broadcast(TagContainer);
}

void UAuraAbilitySystemComponent::Server_UpgradeAttribute_Implementation(const FGameplayTag& AttributeTag)
{
	FGameplayEventData PayLoad;
	PayLoad.EventTag = AttributeTag;
	PayLoad.EventMagnitude = 1.f;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, PayLoad);

	if (GetAvatarActor() && GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::Client_UpdateAbilityStatus_Implementation(
	const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::Server_SpendSpellPoint_Implementation(const FGameplayTag& AbilityTag)
{
	if (!GetAvatarActor()->Implements<UPlayerInterface>()) return;
	IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
	
	if (const auto AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const auto GameplayTags = FAuraGameplayTags::Get();
		auto Status = GetStatusTagFromSpec(*AbilitySpec);

		if (Status.MatchesTagExact(GameplayTags.Ability_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(GameplayTags.Ability_Status_Eligible);
			AbilitySpec->DynamicAbilityTags.AddTag(GameplayTags.Ability_Status_Unlocked);
			Status = GameplayTags.Ability_Status_Unlocked;
		}
		else if (Status.MatchesTagExact(GameplayTags.Ability_Status_Equipped) ||
				 Status.MatchesTagExact(GameplayTags.Ability_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}
		Client_UpdateAbilityStatus(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::MultiCast_ActivatePassiveEffect_Implementation(const FGameplayTag& AbilityTag, const bool bActivate)
{
	OnActivePassiveEffectDelegate.Broadcast(AbilityTag, bActivate);
}