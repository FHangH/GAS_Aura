// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

UCLASS()
class GAS_AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	bool bIsHighLight {false};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Enemy", meta=(AllowPrivateAccess=true))
	int32 Level {1};
	
	/* Function */
public:
	AAuraEnemy();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;
	
public:
	// Enemy Interface
	virtual void HighLightActor() override;
	virtual void UnHighLightActor() override;

	// Combat Interface
	virtual int32 GetPlayerLevel() override;
};
