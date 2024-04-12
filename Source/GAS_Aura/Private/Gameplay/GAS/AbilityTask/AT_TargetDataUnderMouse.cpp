// Copyright fangh.space


#include "Gameplay/GAS/AbilityTask/AT_TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

void UAT_TargetDataUnderMouse::Activate()
{
	if (!AbilitySystemComponent.IsValid() || !Ability) return;
	if (Ability->GetCurrentActorInfo()->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		AbilitySystemComponent.Get()->
			AbilityTargetDataSetDelegate(GetAbilitySpecHandle(), GetActivationPredictionKey()).
				AddUObject(this, &UAT_TargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const auto bCalledDelegate = AbilitySystemComponent.Get()->
			CallReplicatedTargetDataDelegatesIfSet(GetAbilitySpecHandle(), GetActivationPredictionKey());
		
		if (!bCalledDelegate)
		{
			SetWaitingOnRemotePlayerData();
		}
	}
}

UAT_TargetDataUnderMouse* UAT_TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	const auto MyObject = NewAbilityTask<UAT_TargetDataUnderMouse>(OwningAbility);
	return MyObject;
}

void UAT_TargetDataUnderMouse::SendMouseCursorData() const
{
	auto PC = Ability->GetCurrentActorInfo()->PlayerController.Get();
	if (!PC || !AbilitySystemComponent.IsValid()) return;

	FScopedPredictionWindow ScopedPrediction(AbilitySystemComponent.Get());
	
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
	FGameplayAbilityTargetDataHandle DataHandle;
	auto Data = new FGameplayAbilityTargetData_SingleTargetHit();
	
	Data->HitResult = HitResult;
	DataHandle.Add(Data);

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag{},
		AbilitySystemComponent->ScopedPredictionKey);

	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}

void UAT_TargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle,
	const FGameplayTag ActivationTag) const
{
	if (!AbilitySystemComponent.IsValid()) return;
	
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		ValidData.Broadcast(DataHandle);
	}
}
