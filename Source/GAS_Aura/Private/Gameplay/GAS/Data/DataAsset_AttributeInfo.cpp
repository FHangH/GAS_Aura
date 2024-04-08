// Copyright fangh.space


#include "Gameplay/GAS/Data/DataAsset_AttributeInfo.h"
#include "Untils/AuraLog.h"

FAuraAttributeInfo UDataAsset_AttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag,
                                                                     bool bLogNotFound) const
{
	for (const auto& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}
	if (bLogNotFound)
	{
		UE_LOG(Aura, Error, TEXT("Can't Find info for AttributeTag: [%s] on AttributeInfo: [%s]"), *AttributeTag.ToString(), *GetNameSafe(this));
	}
	return FAuraAttributeInfo{};
}
