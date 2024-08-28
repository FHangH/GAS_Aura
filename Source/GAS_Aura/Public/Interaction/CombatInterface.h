// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

enum class ECharacterClassType : uint8;
class UNiagaraSystem;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnASComponentRegisteredSignature, UAbilitySystemComponent*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeathSignature, AActor*, DeadActor);

USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag SocketTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound;
};

UINTERFACE(MinimalAPI, Blueprintable, BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API ICombatInterface
{
	GENERATED_BODY()

public:
	virtual FOnASComponentRegisteredSignature GetOnAsComponentRegisteredDelegate() = 0;
	virtual FOnDeathSignature GetOnDeathDelegate() = 0;
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Combat")
	int32 GetPlayerLevel();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Combat")
	FVector GetCombatSocketLocation(const FGameplayTag& MontageTag) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Interface|Combat")
	void UpdateFacingTarget(const FVector& TargetLocation);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	bool IsDead() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	AActor* GetAvatar();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	TArray<FTaggedMontage> GetAttackMontages();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	UNiagaraSystem* GetBloodEffect() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	int32 GetMinionCount() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	void IncrementMinionCount(const int32 Amount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Combat")
	ECharacterClassType GetCharacterClassType();
};
