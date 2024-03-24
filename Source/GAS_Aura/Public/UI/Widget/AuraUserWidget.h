// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

UCLASS()
class GAS_AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UObject> WidgetController;

	/* Function */
protected:
	UFUNCTION(BlueprintNativeEvent, Category="Aura")
	void WidgetControllerSet();

public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void SetWidgetController(UObject* InWidgetController);
};