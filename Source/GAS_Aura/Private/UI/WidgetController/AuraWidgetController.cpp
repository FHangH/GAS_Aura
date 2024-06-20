// Copyright fangh.space


#include "UI/WidgetController/AuraWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AbilityInfo.h"
#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"

AAuraPlayerController* UAuraWidgetController::GetAuraPC()
{
	if (AuraPlayerController) return AuraPlayerController;
	return AuraPlayerController = Cast<AAuraPlayerController>(PlayerController);
}

AAuraPlayerState* UAuraWidgetController::GetAuraPS()
{
	if (AuraPlayerState) return AuraPlayerState;
	return AuraPlayerState = Cast<AAuraPlayerState>(PlayerState);
}

UAuraAbilitySystemComponent* UAuraWidgetController::GetAuraASC()
{
	if (AuraASComponent) return AuraASComponent;
	return AuraASComponent = Cast<UAuraAbilitySystemComponent>(ASComponent);
}

UAuraAttributeSet* UAuraWidgetController::GetAuraAS()
{
	if (AuraAS) return AuraAS;
	return AuraAS = Cast<UAuraAttributeSet>(AS);
}

void UAuraWidgetController::BindCallBackToDependencies()
{
	
}

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState; 
	ASComponent = Params.ASComponent;
	AS = Params.AS;
}

void UAuraWidgetController::BroadcastInitValues()
{
	
}

void UAuraWidgetController::BroadcastAbilityInfo()
{
	if (!GetAuraASC()->bStartupAbilitiesGiven) return;

	FForEachAbilityDelegate ForEachAbilityDelegate;
	ForEachAbilityDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
	{
		auto DA_AbilityInfo = DataAsset_AbilityInfo->FindAbilityInfoForTag(AuraASComponent->GetAbilityTagFromSpec(AbilitySpec));
		DA_AbilityInfo.InputTag = AuraASComponent->GetInputTagFromSpec(AbilitySpec);
		DA_AbilityInfo.StatusTag = AuraASComponent->GetStatusTagFromSpec(AbilitySpec);
		OnAbilityInfoDelegate.Broadcast(DA_AbilityInfo);
	});
	
	AuraASComponent->ForEachAbility(ForEachAbilityDelegate);
}
