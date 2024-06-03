// Copyright fangh.space


#include "AsyncTasks/Async_WaitCooldownChange.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"

UAsync_WaitCooldownChange* UAsync_WaitCooldownChange::Async_WaitCooldownChange(
	UAbilitySystemComponent* ASC, const FGameplayTag InCooldownTag)
{
	const auto WaitCooldownChange = NewObject<UAsync_WaitCooldownChange>();
	if (!ASC || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}
	WaitCooldownChange->AuraASC = ASC;
	WaitCooldownChange->CooldownTag = InCooldownTag;

	// To know when a cooldown effect has been applied
	ASC->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(WaitCooldownChange, &UAsync_WaitCooldownChange::OnActiveEffectAdded);
	// To know when a cooldown has ended ( cooldown tags has been removed)
	ASC->RegisterGameplayTagEvent(
		InCooldownTag,
		EGameplayTagEventType::NewOrRemoved).AddUObject(WaitCooldownChange, &UAsync_WaitCooldownChange::OnCooldownTagChanged);
	
	return WaitCooldownChange;
}

void UAsync_WaitCooldownChange::EndTask()
{
	if (!AuraASC) return;
	AuraASC->RegisterGameplayTagEvent(CooldownTag, EGameplayTagEventType::NewOrRemoved).RemoveAll(this);
	SetReadyToDestroy();
	MarkAsGarbage();
}

void UAsync_WaitCooldownChange::OnActiveEffectAdded(
	UAbilitySystemComponent* TargetASC, const FGameplayEffectSpec& GESpecApplied, FActiveGameplayEffectHandle ActiveGEHandle)
{
	if (!TargetASC) return;

	FGameplayTagContainer AssetTags;
	GESpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	GESpecApplied.GetAllGrantedTags(GrantedTags);

	if (AssetTags.HasTagExact(CooldownTag) || GrantedTags.HasTagExact(CooldownTag))
	{
		const auto GameplayEffectQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
		const auto TimeRemainingArr = AuraASC->GetActiveEffectsTimeRemaining(GameplayEffectQuery);

		if (TimeRemainingArr.Num() > 0)
		{
			float TimeRemaining = TimeRemainingArr[0];
			
			for (const auto& i : TimeRemainingArr)
			{
				i > TimeRemaining ? TimeRemaining = i : TimeRemaining;
			}
			CooldownStartDelegate.Broadcast(TimeRemaining);
		}
	}
}

void UAsync_WaitCooldownChange::OnCooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount) const
{
	if (NewCount == 0)
	{
		CooldownEndDelegate.Broadcast(0.f);
	}
}
