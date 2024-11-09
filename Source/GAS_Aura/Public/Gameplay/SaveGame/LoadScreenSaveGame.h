// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/SaveGame.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus : uint8
{
	ESSS_Vacant		UMETA(DisplayName="Vacant"),
	ESSS_EnterName	UMETA(DisplayName="EnterName"),
	ESSS_Taken		UMETA(DisplayName="Taken"),
};

USTRUCT(BlueprintType)
struct FSavedActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Actor")
	FName ActorName {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Actor")
	FTransform Transform {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Actor")
	TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT(BlueprintType)
struct FSavedMap
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Map")
	FString MapAssetName {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Map")
	TArray<FSavedActor> SavedActors;
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	TSubclassOf<UGameplayAbility> GameplayAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	FGameplayTag AbilityTag {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	FGameplayTag AbilityStatus {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	FGameplayTag AbilitySlot {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	FGameplayTag AbilityType {};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Ability")
	int32 AbilityLevel { 1 };
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

UCLASS()
class GAS_AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/* Property */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString SlotName {};

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	int32 SlotIndex { 0 };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString PlayerName { "Default Name" };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FString MapName { "Default Map" };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	FName PlayerStartTag;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	TEnumAsByte<ESaveSlotStatus> SlotStatus { ESaveSlotStatus::ESSS_Vacant };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|LoadScreen")
	bool IsFirstTimeLoadIn { true };

	// Player
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Player")
	int32 PlayerLevel { 1 };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Player")
	int32 XP { 0 };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Player")
	int32 SpellPoints { 0 };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Player")
	int32 AttributePoints { 0 };

	// Attributes
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Attributes")
	float Strength { 0.f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Attributes")
	float Intelligence { 0.f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Attributes")
	float Resilience { 0.f };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Attributes")
	float Vigor { 0.f };

	// Abilities
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Abilities")
	TArray<FSavedAbility> SavedAbilities;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category="SaveGame|Abilities")
	TArray<FSavedMap> SavedMaps;

	/* Function */
	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
};
