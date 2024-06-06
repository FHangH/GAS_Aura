// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

UCLASS()
class GAS_AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

	/* Function */
public:
	AAuraCharacter();

protected:
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	// Combat Interface
	virtual int32 GetPlayerLevel_Implementation() override;;

	// Player Interface
	virtual void AddToXP_Implementation(const int32 InXP) override;
	virtual void LevelUp_Implementation() override;

private:
	virtual void InitAbilityActorInfo() override;
};
