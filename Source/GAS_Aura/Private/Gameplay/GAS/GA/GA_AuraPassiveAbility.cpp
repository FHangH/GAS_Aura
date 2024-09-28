// Copyright fangh.space


#include "Gameplay/GAS/GA/GA_AuraPassiveAbility.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"

void UGA_AuraPassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                             const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (const auto AuraASC = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo())))
	{
		AuraASC->OnDeactivatePassiveAbilityDelegate.AddUObject(this, &ThisClass::ReceiveDeactivate);
	}
}

void UGA_AuraPassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (AbilityTags.HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
