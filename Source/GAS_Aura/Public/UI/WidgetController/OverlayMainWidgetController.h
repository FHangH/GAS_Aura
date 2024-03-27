// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayMainWidgetController.generated.h"

struct FOnAttributeChangeData;
// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChangedSignature, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChangedSignature, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnManaChangedSignature, float, NewMana);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxManaChangedSignature, float, NewMaxMana);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UOverlayMainWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnHealthChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnMaxHealthChangedSignature OnMaxHealthChanged;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnManaChangedSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnMaxManaChangedSignature OnMaxManaChanged;

	/* Function */
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallBackToDependencies() override;

protected:
	void HealthChanged(const FOnAttributeChangeData& Data) const;
	void MaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void ManaChanged(const FOnAttributeChangeData& Data) const;
	void MaxManaChanged(const FOnAttributeChangeData& Data) const;
};
