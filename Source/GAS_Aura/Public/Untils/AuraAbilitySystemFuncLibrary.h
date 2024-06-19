// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemFuncLibrary.generated.h"

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
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|PlayerController", meta=(DefaultToSelf="WorldContextObject"))
	static AAuraPlayerController* GetAuraPlayerController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static bool MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWidgetParams, AAuraHUD*& OutAuraHUD);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UOverlayMainWidgetController* GetOverlayMainWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController", meta=(DefaultToSelf="WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClassType ECT, const float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|Abilities")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, const ECharacterClassType ECT);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static UDataAsset_CharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayEffect")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsBlockedHit);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|GameplayEffect")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, const bool bIsCriticalHit);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingPlayers, const TArray<AActor*>& ActorsToIgnore, const float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|GameplayMechanics")
	static bool IsNotFriend(const AActor* FirstActor, const AActor* SecondActor);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|XPReward")
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, const ECharacterClassType ECT, const int32 Level);
};
