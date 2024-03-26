// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;

UCLASS()
class GAS_AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, Category="Aura|HUD")
	TSubclassOf<UAuraUserWidget> OverlayMainWidgetClass;

public:
	UPROPERTY(BlueprintReadOnly, Category="Aura|HUD")
	TObjectPtr<UAuraUserWidget> OverlayMainWidget;

	/* Function */
protected:
	virtual void BeginPlay() override;
};
