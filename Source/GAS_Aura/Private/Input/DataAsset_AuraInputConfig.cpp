// Copyright fangh.space


#include "Input/DataAsset_AuraInputConfig.h"
#include "Untils/AuraLog.h"

const UInputAction* UDataAsset_AuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (const auto Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(Aura, Error, TEXT("Can't Find AbilityInputAction For InputTag [%s] On InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}
