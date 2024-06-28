// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GA_AuraProjectileSpell.h"
#include "GA_AuraFireBolt.generated.h"

UCLASS()
class GAS_AURA_API UGA_AuraFireBolt : public UGA_AuraProjectileSpell
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura", meta=(AllowPrivateAccess=true))
	int32 NumProjectiles { 5 };
	
	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;
};
