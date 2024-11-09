// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataTable.h"
#include "Gameplay/GAS/AuraAbilitySystemComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayMainWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAuraAbilitySystemComponent;
class UAuraUserWidget;
struct FOnAttributeChangeData;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	FGameplayTag MessageTag {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	FText MessageText {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	TSubclassOf<UAuraUserWidget> MessageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura")
	TObjectPtr<UTexture2D> Image {nullptr};
};

// Delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChangedSignature, int32, NewLevel, bool, IsLevelUp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, FUIWidgetRow, Row);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UOverlayMainWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Attributes")
	FOnAttributeChangedSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Attributes")
	FOnAttributeChangedSignature OnManaChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Attributes")
	FOnAttributeChangedSignature OnMaxManaChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Messages")
	FOnMessageWidgetRowSignature OnMessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Level")
	FOnLevelChangedSignature OnPlayerLevelChangedSignature;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|WidgetController|DataTable")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	/* Function */
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallBackToDependencies() override;

protected:
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnManaChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxManaChanged(const FOnAttributeChangeData& Data) const;
	void OnEffectAssetTag(const FGameplayTagContainer& AssetTags) const;
	void OnXPChanged(const int32 NewXP);
	void OnLevelChanged(const int32 NewLevel, const bool IsLevelUp) const;
	void OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& StatusSlot, const FGameplayTag& SlotTag, const FGameplayTag& PreviousSlot) const;

	template<typename T>
	static T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}
};
