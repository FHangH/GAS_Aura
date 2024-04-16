// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "DamageTextWidgetComponent.generated.h"

UCLASS(ClassGroup=(Aura), meta=(BlueprintSpawnableComponent))
class GAS_AURA_API UDamageTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UDamageTextWidgetComponent();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Aura|WidgetComponent")
	void SetDamageText(const float Damage);
};
