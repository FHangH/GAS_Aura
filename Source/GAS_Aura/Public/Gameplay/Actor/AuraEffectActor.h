// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnBeginOverlap,
	ApplyOnEndOverlap,
	DoNotApply
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove
};

UCLASS()
class GAS_AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

	/* Property */
private:
	// Enum Apply GE Policy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Policy|Apply", meta=(AllowPrivateAccess=true))
	EEffectApplicationPolicy InstantGE_ApplicationPolicy {EEffectApplicationPolicy::DoNotApply};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Policy|Apply", meta=(AllowPrivateAccess=true))
	EEffectApplicationPolicy DurationGE_ApplicationPolicy {EEffectApplicationPolicy::DoNotApply};;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Policy|Apply", meta=(AllowPrivateAccess=true))
	EEffectApplicationPolicy InfinityGE_ApplicationPolicy {EEffectApplicationPolicy::DoNotApply};;

	// Enum Remove GE Policy
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Policy|Remove", meta=(AllowPrivateAccess=true))
	EEffectRemovalPolicy InfinityGE_RemovalPolicy {EEffectRemovalPolicy::RemoveOnEndOverlap};

	// GameplayEffect Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|GE", meta=(AllowPrivateAccess=true))
	TSubclassOf<UGameplayEffect> InfinityGameplayEffectClass;

	// All Active GE Handle Map
	TMap<FActiveGameplayEffectHandle, UAbilitySystemComponent*> ActiveGEHandles_Map;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|Level", meta=(AllowPrivateAccess=true))
	float ActorLevel {1.f};
	
	/* Function */
public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Aura")
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UFUNCTION(BlueprintCallable, Category="Aura")
	void OnBeginOverlap(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, Category="Aura")
	void OnEndOverlap(AActor* TargetActor);
};