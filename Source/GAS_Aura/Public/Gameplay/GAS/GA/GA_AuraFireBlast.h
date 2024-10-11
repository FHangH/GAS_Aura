// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/GAS/AuraDamageGameplayAbility.h"
#include "GA_AuraFireBlast.generated.h"

class AAuraFireBall;

UCLASS()
class GAS_AURA_API UGA_AuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Aura|FireBlast")
	int32 NumFireBalls { 12 };

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aura|FireBlast", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AAuraFireBall> FireBallClass;
	
	/* Function */
public:
	virtual FString GetDescription(const int32 Level) override;
	virtual FString GetNextLevelDescription(const int32 Level) override;

	UFUNCTION(BlueprintCallable, Category = "Aura|FireBlast")
	TArray<AAuraFireBall*> SpawnFireBalls();
};
