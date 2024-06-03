// Copyright fangh.space


#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Untils/AuraLog.h"

FAuraAbilityInfo UDataAsset_AbilityInfo::FindAbilityInfoForTag(const FGameplayTag& AbilityTag, const bool bLogNotFound) const
{
	for (const auto& Info : AbilityInformation)
	{
		if (Info.AbilityTag.MatchesTagExact(AbilityTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(Aura, Error, TEXT("Can't Find info for AbilityTag: [%s] on AbilityInfo: [%s]"), *AbilityTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAbilityInfo{};
}
