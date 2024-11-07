// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);

UCLASS()
class GAS_AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable)
	FSetWidgetSwitcherIndexSignature SetWidgetSwitcherIndexDelegate;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot")
	FString LoadSlotName;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot")
	FString SlotIndex;

	/* Function */
public:
	void InitializeSlot() const;
};
