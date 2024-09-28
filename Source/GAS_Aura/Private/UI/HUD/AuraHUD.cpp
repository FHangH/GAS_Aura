// Copyright fangh.space


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "Untils/AuraLog.h"

UOverlayMainWidgetController* AAuraHUD::GetOverlayMainWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayMainWidgetController) return OverlayMainWidgetController;

	if (IsValid(OverlayMainWidgetControllerClass))
	{
		OverlayMainWidgetController = NewObject<UOverlayMainWidgetController>(this, OverlayMainWidgetControllerClass);
		OverlayMainWidgetController->SetWidgetControllerParams(Params);
		OverlayMainWidgetController->BindCallBackToDependencies();
		return OverlayMainWidgetController;
	}
	
	UE_LOG(Aura, Warning, TEXT("OverlayMainWidgetControllerClass is nullptr"));
	return nullptr;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (AttributeMenuWidgetController) return AttributeMenuWidgetController;

	if (IsValid(AttributeMenuWidgetControllerClass))
	{
		AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(this, AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(Params);
		AttributeMenuWidgetController->BindCallBackToDependencies();
		return AttributeMenuWidgetController;
	}
	
	UE_LOG(Aura, Warning, TEXT("AttributeMenuWidgetControllerClass is nullptr"));
	return nullptr;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& Params)
{
	if (SpellMenuWidgetController) return SpellMenuWidgetController;

	if (IsValid(SpellMenuWidgetControllerClass))
	{
		SpellMenuWidgetController = NewObject<USpellMenuWidgetController>(this, SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(Params);
		SpellMenuWidgetController->BindCallBackToDependencies();
		return SpellMenuWidgetController;
	}

	UE_LOG(Aura, Warning, TEXT("SpellMenuWidgetControllerClass is nullptr"));
	return nullptr;
}

void AAuraHUD::InitOverlayMain(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (IsValid(OverlayMainWidgetClass))
	{
		if (const auto Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayMainWidgetClass))
		{
			OverlayMainWidget = Cast<UAuraUserWidget>(Widget);
			if (OverlayMainWidget)
			{
				const FWidgetControllerParams WidgetControllerParams {PC, PS, ASC, AS};
				const auto WidgetController = GetOverlayMainWidgetController(WidgetControllerParams);

				OverlayMainWidget->SetWidgetController(WidgetController);
				WidgetController->BroadcastInitValues();
				Widget->AddToViewport();
			}
		}
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("OverlayMainWidgetClass is nullptr"));
	}
}
