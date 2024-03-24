// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()

	/* Function */
public:
	AAuraCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	void InitAbilityActorInfo();
};
