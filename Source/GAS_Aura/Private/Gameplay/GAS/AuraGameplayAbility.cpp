// Copyright fangh.space


#include "Gameplay/GAS/AuraGameplayAbility.h"

FString UAuraGameplayAbility::GetDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - XXXXXXXXXXXXXXXX", Level);
}

FString UAuraGameplayAbility::GetNextLevelDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Next Level: </><Level>%d</> \n<Default>Causes much more damage. </>"), Level);
}

FString UAuraGameplayAbility::GetLockedDescription(const int32 Level)
{
	return FString::Printf(TEXT("<Default>Spell Locked Until Level: </><Level>%d</>"), Level);
}
