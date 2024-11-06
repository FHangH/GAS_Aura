// Copyright fangh.space


#include "UI/MVVM/MVVM_LoadScreen.h"
#include "UI/MVVM/MVVM_LoadSlot.h"

void UMVVM_LoadScreen::InitializeLoadSlots()
{
	if (MVVM_LoadSlotClass)
	{
		LoadSlot_0 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		LoadSlot_1 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		LoadSlot_2 = NewObject<UMVVM_LoadSlot>(this, MVVM_LoadSlotClass);
		Map_LoadSlots.Add(0, LoadSlot_0);
		Map_LoadSlots.Add(1, LoadSlot_0);
		Map_LoadSlots.Add(2, LoadSlot_0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MVVM_LoadSlotClass is null"));
	}
}

UMVVM_LoadSlot* UMVVM_LoadScreen::GetMVVM_LoadSlotByIndex(const int32 Index) const
{
	return Map_LoadSlots.FindChecked(Index);
}
