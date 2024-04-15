// Copyright fangh.space


#include "Gameplay/GAS/Data/DataAsset_CharacterClassInfo.h"

FCharacterClassDefaultInfo UDataAsset_CharacterClassInfo::GetClassDefaultInfo(const ECharacterClassType ECT)
{
	return Map_CharacterClassInformation.FindChecked(ECT);
}
