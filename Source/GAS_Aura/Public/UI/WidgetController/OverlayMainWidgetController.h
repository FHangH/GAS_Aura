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
class UDataAsset_AbilityInfo;
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);

UCLASS(BlueprintType, Blueprintable)
class GAS_AURA_API UOverlayMainWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnAttributeChangedSignature OnHealthChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnAttributeChangedSignature OnMaxHealthChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnAttributeChangedSignature OnManaChangedDelegate;
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnAttributeChangedSignature OnMaxManaChangedDelegate;
	
	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FOnMessageWidgetRowSignature OnMessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category="Aura|Delegate")
	FAbilityInfoSignature AbilityInfoDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|DataTable")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|DataAsset")
	TObjectPtr<UDataAsset_AbilityInfo> DataAsset_AbilityInfo;

	/* Function */
public:
	virtual void BroadcastInitValues() override;
	virtual void BindCallBackToDependencies() override;

protected:
	void OnHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data) const;
	void OnManaChanged(const FOnAttributeChangeData& Data) const;
	void OnMaxManaChanged(const FOnAttributeChangeData& Data) const;
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC) const;
	void OnEffectAssetTag(const FGameplayTagContainer& AssetTags) const;

	template<typename T>
	static T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag)
	{
		return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
	}
};
