// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "ScalableFloat.h"
#include "Engine/DataAsset.h"
#include "DataAsset_CharacterClassInfo.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM(BlueprintType)
enum class ECharacterClassType : uint8
{
	ECT_Elementalist	UMETA(Displayname="Elementalist"),
	ECT_Warrior			UMETA(Displayname="Warrior"),
	ECT_Ranger			UMETA(Displayname="Ranger")
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	TSubclassOf<UGameplayEffect> GE_PrimaryAttributes;
	
	UPROPERTY(EditDefaultsOnly, Category="Aura")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Aura")
	FScalableFloat XPReward {};
};

UCLASS(BlueprintType)
class GAS_AURA_API UDataAsset_CharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="Aura Character Info")
	TMap<ECharacterClassType, FCharacterClassDefaultInfo> Map_CharacterClassInformation;

	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Attributes")
	TSubclassOf<UGameplayEffect> GE_PrimaryAttributes_SetByCaller;
	
	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Attributes")
	TSubclassOf<UGameplayEffect> GE_SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Attributes")
	TSubclassOf<UGameplayEffect> GE_SecondaryAttributes_Infinite;
	
	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Attributes")
	TSubclassOf<UGameplayEffect> GE_VitalAttributes;

	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Aura Common|Damage")
	TObjectPtr<UCurveTable> DamageCalculationCoefficient;
	
	FCharacterClassDefaultInfo GetClassDefaultInfo(const ECharacterClassType ECT);
};
