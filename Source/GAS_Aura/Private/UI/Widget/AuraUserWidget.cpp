// Copyright fangh.space


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::OnWidgetControllerSet_Implementation()
{
}

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
