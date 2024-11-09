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

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	FString LoadSlotName;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	int32 SlotIndex;

	UPROPERTY(BlueprintReadWrite, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	FName PlayerStartTag;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	FString PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	FString MapName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, FieldNotify, Setter, Getter, Category="MVVM|LoadSlot", meta=(AllowPrivateAccess=true))
	int32 PlayerLevel;

	/* Function */
public:
	void InitializeSlot() const;

	// Field Notifies
	void SetPlayerName(const FString& InPlayerName);
	void SetMapName(const FString& InMapName);
	void SetPlayerLevel(const int32 InPlayerLevel);
	FString GetPlayerName() const { return PlayerName; }
	FString GetMapName() const { return MapName; }
	int32 GetPlayerLevel() const { return PlayerLevel; }
};
