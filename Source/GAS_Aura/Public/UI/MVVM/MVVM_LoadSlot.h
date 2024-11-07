// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadSlot.generated.h"

enum ESaveSlotStatus : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetWidgetSwitcherIndexSignature, int32, WidgetSwitcherIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEnableSelectSlotButtonSignature, bool, bEnable);

UCLASS()
class GAS_AURA_API UMVVM_LoadSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(BlueprintAssignable, Category="MVVM|LoadSlot")
	FSetWidgetSwitcherIndexSignature SetWidgetSwitcherIndexDelegate;

	UPROPERTY(BlueprintAssignable, Category="MVVM|LoadSlot")
	FEnableSelectSlotButtonSignature EnableSelectSlotButtonDelegate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="MVVM|LoadSlot")
	TEnumAsByte<ESaveSlotStatus> SlotStatus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category="MVVM|LoadSlot")
	FString PlayerName;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot")
	FString LoadSlotName;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot")
	int32 SlotIndex;

	/* Function */
public:
	void InitializeSlot() const;

	// Field Notifies
	void SetPlayerName(const FString& InPlayerName);
	FString GetPlayerName() const { return PlayerName; }
};
