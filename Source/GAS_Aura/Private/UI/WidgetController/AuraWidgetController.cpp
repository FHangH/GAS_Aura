// Copyright fangh.space


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	ASComponent = Params.ASComponent;
	AS = Params.AS;
}

void UAuraWidgetController::BroadcastInitValues()
{
	
}

void UAuraWidgetController::BindCallBackToDependencies()
{
	
}
