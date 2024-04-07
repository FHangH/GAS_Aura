// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemFuncLibrary.generated.h"

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
};
