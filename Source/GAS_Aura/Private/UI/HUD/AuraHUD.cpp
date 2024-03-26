// Copyright fangh.space


#include "UI/HUD/AuraHUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "Untils/AuraLog.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	if (OverlayMainWidgetClass)
	{
		OverlayMainWidget = CreateWidget<UAuraUserWidget>(GetWorld(), OverlayMainWidgetClass);
		if (OverlayMainWidget)
		{
			const auto Widget = Cast<UUserWidget>(OverlayMainWidget);
			Widget->AddToViewport();
		}
	}
	else
	{
		UE_LOG(Aura, Warning, TEXT("OverlayMainWidgetClass is nullptr"));
	}
}
