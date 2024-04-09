// Copyright fangh.space


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AttributeInfo.h"
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
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag,
															const FGameplayAttribute& Attribute) const
{
	auto Info = DA_AttributeInfo->FindAttributeInfoForTag(AttributeTag, true);
	Info.AttributeValue = Attribute.GetNumericValue(AS);
	AttributeInfoSignature.Broadcast(Info);
}
