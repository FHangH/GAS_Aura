// Copyright fangh.space


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AttributeInfo.h"
#include "Gameplay/PlayerState/AuraPlayerState.h"
#include "Untils/AuraLog.h"

void UAttributeMenuWidgetController::BindCallBackToDependencies()
{
	const auto AuraAS = CastChecked<UAuraAttributeSet>(AS);

	for (const auto& Item : AuraAS->Map_TagsToAttributes)
	{
		ASComponent->GetGameplayAttributeValueChangeDelegate(Item.Value()).AddLambda
		(
			[this, Item](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Item.Key, Item.Value());
			}
		);
	}

	if (const auto AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		AuraPlayerState->OnAttributePointChangedDelegate.AddLambda
		([this](const int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		});
	}
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	if (!DA_AttributeInfo)
	{
		UE_LOG(Aura, Warning, TEXT("DataAsset_AttributeInfo Is Null"));
		return;
	}

	const auto AuraAS = CastChecked<UAuraAttributeSet>(AS);

	for (const auto& Item : AuraAS->Map_TagsToAttributes)
	{
		BroadcastAttributeInfo(Item.Key, Item.Value());
	}

	if (const auto AuraPlayerState = Cast<AAuraPlayerState>(PlayerState))
	{
		AttributePointsChangedDelegate.Broadcast(AuraPlayerState->GetAttributePoints());
	}
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if (const auto AuraASC = Cast<UAuraAbilitySystemComponent>(ASComponent))
	{
		AuraASC->UpgradeAttribute(AttributeTag);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
                                                            const FGameplayAttribute& Attribute) const
{
	auto Info = DA_AttributeInfo->FindAttributeInfoForTag(AttributeTag, true);
	Info.AttributeValue = Attribute.GetNumericValue(AS);
	AttributeInfoSignature.Broadcast(Info);
}
