// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;
};
