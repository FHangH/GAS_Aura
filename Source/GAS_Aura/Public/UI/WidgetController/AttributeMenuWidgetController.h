// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;
};
