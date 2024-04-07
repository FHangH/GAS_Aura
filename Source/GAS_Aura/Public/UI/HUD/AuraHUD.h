// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AuraHUD.generated.h"

class UAttributeMenuWidgetController;
class UAttributeSet;
class UAbilitySystemComponent;
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
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayMainWidget;
	
	UPROPERTY(EditAnywhere, Category="Aura|HUD")
	TSubclassOf<UOverlayMainWidgetController> OverlayMainWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UOverlayMainWidgetController> OverlayMainWidgetController;

	UPROPERTY(EditAnywhere, Category="Aura|HUD")
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	/* Function */
public:
	UOverlayMainWidgetController* GetOverlayMainWidgetController(const FWidgetControllerParams& Params);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParams& Params);

	void InitOverlayMain(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
};
