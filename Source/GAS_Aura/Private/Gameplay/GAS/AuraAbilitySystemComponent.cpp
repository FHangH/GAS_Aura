// Copyright fangh.space


#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/GAS/AuraGameplayAbility.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
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
		GiveAbilityAndActivateOnce(GASpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if (!InputTag.IsValid()) return;

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

	for (auto& GASpec : GetActivatableAbilities())
	{
		if (!GASpec.DynamicAbilityTags.HasTagExact(InputTag)) continue;
		
		AbilitySpecInputReleased(GASpec);
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbilityDelegate& Delegate)
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
			Client_UpdateAbilityStatus(Info.AbilityTag, FAuraGameplayTags::Get().Ability_Status_Eligible);
		}
	}
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
	const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag)
{
	OnAbilityStatusChangedDelegate.Broadcast(AbilityTag, StatusTag);
}