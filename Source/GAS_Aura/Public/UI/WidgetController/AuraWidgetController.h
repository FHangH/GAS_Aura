// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class GAS_AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAttributeSet> AS;
};
