// Copyright fangh.space


#include "Untils/AuraAbilitySystemFuncLibrary.h"
#include "AbilitySystemComponent.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayMainWidgetController* UAuraAbilitySystemFuncLibrary::GetOverlayMainWidgetController(
																const UObject* WorldContextObject)
{
	const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (!PC) return nullptr;

	const auto AuraHUD = Cast<AAuraHUD>(PC->GetHUD());
	if (!AuraHUD) return nullptr;

	const auto PS = PC->GetPlayerState<AAuraPlayerState>();
	if (!PS) return nullptr;

	const auto ASC = PS->GetAbilitySystemComponent();
	if (!ASC) return nullptr;

	const auto AS = PS->GetAttributeSet();
	if (!AS) return nullptr;

	const FWidgetControllerParams WidgetControllerParams {PC, PS, ASC, AS};
	return AuraHUD->GetOverlayMainWidgetController(WidgetControllerParams);
}
