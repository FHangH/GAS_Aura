// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
struct FWidgetControllerParams;
class UOverlayMainWidgetController;
class UAuraUserWidget;

UCLASS()
class GAS_AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, Category="Aura|HUD")
	TSubclassOf<UAuraUserWidget> OverlayMainWidgetClass;

	UPROPERTY(EditAnywhere, Category="Aura|HUD")
	TSubclassOf<UOverlayMainWidgetController> OverlayMainWidgetControllerClass;
	
	UPROPERTY()
	TObjectPtr<UOverlayMainWidgetController> OverlayMainWidgetController;

public:
	UPROPERTY(BlueprintReadOnly, Category="Aura|HUD")
	TObjectPtr<UAuraUserWidget> OverlayMainWidget;

	/* Function */
public:
	UOverlayMainWidgetController* GetOverlayMainWidgetController(const FWidgetControllerParams& Params);

	void InitOverlayMain(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
};
