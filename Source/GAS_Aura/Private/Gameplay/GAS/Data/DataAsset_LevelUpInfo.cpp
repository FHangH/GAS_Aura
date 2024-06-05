// Copyright fangh.space


#include "Gameplay/GAS/Data/DataAsset_LevelUpInfo.h"

int32 UDataAsset_LevelUpInfo::FindLevelForXP(const int32 XP) const
{
	int32 level = 1;
	bool isSearching = true;

	while (isSearching)
	{
		if (LevelUpInformation.Num() - 1 <= level) return level;

		if (XP >= LevelUpInformation[level].LevelUpRequirement)
		{
			++level;
		}
		else
		{
			isSearching = false;
		}
	}
	return level;
}
