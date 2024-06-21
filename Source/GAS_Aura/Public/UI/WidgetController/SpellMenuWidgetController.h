// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "SpellMenuWidgetController.generated.h"

struct FGameplayTag;

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API USpellMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Messages")
	FOnPlayerStatusChangedSignature OnSpellPointsChangedDelegate;

	/* Function */
public:
	virtual void BindCallBackToDependencies() override;
	virtual void BroadcastInitValues() override;

protected:
	void OnAbilityStatusChanged(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) const;
	void OnSpellPointsChanged(const int32 SpellPoints) const;
};
