// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class GAS_AURA_API IPlayerInterface
{
	GENERATED_BODY()

	/* Function */
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 FindLevelForXP(const int32 InXP) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 GetAttributePointReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 GetSpellPointReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void AddToXP(const int32 InXP);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void AddToPlayerLevel(const int32 InPlayerLevel);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void AddToAttributePoints(const int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void AddToSpellPoints(const int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interface|Player")
	void LevelUp();
};
