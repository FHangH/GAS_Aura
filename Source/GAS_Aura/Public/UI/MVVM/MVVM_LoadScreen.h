// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSlotSelectedSignature);

UCLASS()
class GAS_AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="MVVM")
	TSubclassOf<UMVVM_LoadSlot> MVVM_LoadSlotClass;

	UPROPERTY(BlueprintAssignable, Category="MVVM")
	FSlotSelectedSignature SlotSelectedDelegate;

private:
	UPROPERTY(BlueprintReadWrite, Category="MVVM", meta=(AllowPrivateAccess=true))
	TMap<int32, UMVVM_LoadSlot*> Map_LoadSlots;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_0;
	
	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_1;

	UPROPERTY()
	TObjectPtr<UMVVM_LoadSlot> LoadSlot_2;
	
	/* Function */
public:
	void InitializeLoadSlots();

	UFUNCTION(BlueprintPure, Category="MVVM")
	UMVVM_LoadSlot* GetMVVM_LoadSlotByIndex(const int32 Index) const;

	UFUNCTION(BlueprintCallable, Category="MVVM")
	void NewSlotButtonPressed(const int32 Slot, const FString& EnteredName);

	UFUNCTION(BlueprintCallable, Category="MVVM")
	void NewGameButtonPressed(const int32 Slot);

	UFUNCTION(BlueprintCallable, Category="MVVM")
	void SelectSlotButtonPressed(const int32 Slot);

	UFUNCTION()
	void LoadData();
};
