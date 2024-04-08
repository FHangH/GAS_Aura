// Copyright fangh.space


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "Gameplay/GAS/AuraAttributeSet.h"
#include "Gameplay/GAS/Data/DataAsset_AttributeInfo.h"
#include "Untils/AuraGameplayTags.h"
#include "Untils/AuraLog.h"

void UAttributeMenuWidgetController::BindCallBackToDependencies()
{
}

void UAttributeMenuWidgetController::BroadcastInitValues()
{
	if (!DA_AttributeInfo)
	{
		UE_LOG(Aura, Warning, TEXT("DataAsset_AttributeInfo Is Null"));
		return;
	}

	const auto AuraAS = CastChecked<UAuraAttributeSet>(AS);
	auto Info = DA_AttributeInfo->FindAttributeInfoForTag(FAuraGameplayTags::Get().Attributes_Primary_Strength, true);
	Info.AttributeValue = AuraAS->GetStrength();
	AttributeInfoSignature.Broadcast(Info);
}
