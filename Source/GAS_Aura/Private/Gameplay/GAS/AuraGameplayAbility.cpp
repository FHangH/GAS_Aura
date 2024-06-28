// Copyright fangh.space


#include "Gameplay/GAS/AuraGameplayAbility.h"
#include "Gameplay/GAS/AuraAttributeSet.h"

float UAuraGameplayAbility::GetManaCost(const float InLevel) const
{
    float ManaCost = 0.f;
	if (const auto CostEffect = GetCostGameplayEffect())
	{
		for (auto& Modifier : CostEffect->Modifiers)
		{
		    if (Modifier.Attribute == UAuraAttributeSet::GetManaAttribute())
            {
                Modifier.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
		    	break;
            }
		}
	}
	return ManaCost;
}

float UAuraGameplayAbility::GetCooldown(const float InLevel) const
{
	float Cooldown = 0.f;
	if (const auto CooldownEffect = GetCooldownGameplayEffect())
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
	}
	return Cooldown;
}

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
