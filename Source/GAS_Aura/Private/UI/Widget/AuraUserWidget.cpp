// Copyright fangh.space


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::WidgetControllerSet_Implementation()
{
}

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
