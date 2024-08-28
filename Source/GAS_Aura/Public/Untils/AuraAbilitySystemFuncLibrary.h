// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilityTypes.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemFuncLibrary.generated.h"

struct FDamageEffectParams;
class AAuraPlayerController;
class AAuraHUD;
struct FWidgetControllerParams;
class USpellMenuWidgetController;
class UDataAsset_CharacterClassInfo;
struct FGameplayEffectContextHandle;
enum class ECharacterClassType : uint8;
class UAbilitySystemComponent;
class UAttributeMenuWidgetController;
class UOverlayMainWidgetController;

UCLASS()
class GAS_AURA_API UAuraAbilitySystemFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* Function */
public:
	// PlayerController
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|PlayerController", meta=(DefaultToSelf="WorldContextObject"))
	static AAuraPlayerController* GetAuraPlayerController(const UObject* WorldContextObject);

	// WidgetController
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetParams, AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayMainWidgetController* GetOverlayMainWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	// CharacterClass TypeInfo
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClassType ECT, const float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static UDataAsset_CharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	// Ability
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|Abilities")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClassType ECT);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|AbilityInfo")
	static UDataAsset_AbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	// Gameplay Effect
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static bool IsSuccessfulDeBuff(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static float GetDeBuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static float GetDeBuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static float GetDeBuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsCriticalHit);
	
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetIsSuccessfulDeBuff(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsSuccessfulDeBuff);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetDeBuffDamage(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDamage);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetDeBuffDuration(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InDuration);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetDeBuffFrequency(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const float InFrequency);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetDamageType(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);

	// Gameplay Mechanics
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingPlayers, const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	// XP Reward
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|XPReward")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, const ECharacterClassType ECT, const int32 Level);

	// ApplyDamage
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|ApplyDamage")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams);
};
