// Copyright fangh.space


#include "UI/MVVM/MVVM_LoadSlot.h"

void UMVVM_LoadSlot::InitializeSlot() const
{
	SetWidgetSwitcherIndexDelegate.Broadcast(SlotStatus.GetValue());
}

// Field Notifies
void UMVVM_LoadSlot::SetPlayerName(const FString& InPlayerName)
{
	UE_MVVM_SET_PROPERTY_VALUE(PlayerName, InPlayerName);
}
