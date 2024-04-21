// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemFuncLibrary.generated.h"

class UAbilitySystemComponent;
enum class ECharacterClassType : uint8;
class UAttributeMenuWidgetController;
class UOverlayMainWidgetController;

UCLASS()
class GAS_AURA_API UAuraAbilitySystemFuncLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/* Function */
public:
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController")
	static UOverlayMainWidgetController* GetOverlayMainWidgetController(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category="Aura|FuncLib|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, const ECharacterClassType ECT, const float Level, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|Abilities")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category="Aura|FuncLib|CharacterClassTypeInfo")
	static UDataAsset_CharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
};
