// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AuraCharacterBase.generated.h"

UCLASS(Abstract)
class GAS_AURA_API AAuraCharacterBase : public ACharacter
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, Category="Aura")
	TObjectPtr<USkeletalMeshComponent> WeaponMeshComponent;

	/* Function */
public:
	AAuraCharacterBase();

protected:
	virtual void BeginPlay() override;
};