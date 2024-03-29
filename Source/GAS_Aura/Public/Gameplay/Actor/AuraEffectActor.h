// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class GAS_AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	/* Function */
public:	
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Aura")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
};