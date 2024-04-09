// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

struct FGameplayAttribute;
struct FGameplayTag;
class UDataAsset_AttributeInfo;
struct FAuraAttributeInfo;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature, const FAuraAttributeInfo&, Info);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable, Category="Aura")
	FAttributeInfoSignature AttributeInfoSignature;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Aura")
	TObjectPtr<UDataAsset_AttributeInfo> DA_AttributeInfo;
	
	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;

private:
	void BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const;
};
