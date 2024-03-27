// Copyright fangh.space


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayMainWidgetController.h"
#include "Untils/AuraLog.h"

UOverlayMainWidgetController* AAuraHUD::GetOverlayMainWidgetController(const FWidgetControllerParams& Params)
{
	if (OverlayMainWidgetController) return OverlayMainWidgetController;

	if (OverlayMainWidgetControllerClass)
	{
		OverlayMainWidgetController =
			NewObject<UOverlayMainWidgetController>(this, OverlayMainWidgetControllerClass);
		OverlayMainWidgetController->SetWidgetControllerParams(Params);
		OverlayMainWidgetController->BindCallBackToDependencies();
		
		return OverlayMainWidgetController;
	}
	
	UE_LOG(Aura, Warning, TEXT("OverlayMainWidgetControllerClass is nullptr"));
	return nullptr;
}

void AAuraHUD::InitOverlayMain(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	if (OverlayMainWidgetClass)
	{
		OverlayMainWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayMainWidgetClass);
		if (OverlayMainWidget)
		{
			const auto Widget = Cast<UUserWidget>(OverlayMainWidget);

			const FWidgetControllerParams WidgetControllerParams {PC, PS, ASC, AS};
			const auto WidgetController = GetOverlayMainWidgetController(WidgetControllerParams);

			OverlayMainWidget->SetWidgetController(WidgetController);
			WidgetController->BroadcastInitValues();
			Widget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("OverlayMainWidgetClass is nullptr"));
	}
}
