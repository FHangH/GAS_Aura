﻿// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_LoadScreen.generated.h"

class UMVVM_LoadSlot;

UCLASS()
class GAS_AURA_API UMVVM_LoadScreen : public UMVVMViewModelBase
{
	GENERATED_BODY()

	/* Property */
public:
	UPROPERTY(EditDefaultsOnly, Category="MVVM")
	TSubclassOf<UMVVM_LoadSlot> MVVM_LoadSlotClass;

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
};