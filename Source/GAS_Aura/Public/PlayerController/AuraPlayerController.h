// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS()
class GAS_AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

	/* Property */
private:
	UPROPERTY(EditAnywhere, Category="Aura|Input")
	TObjectPtr<UInputMappingContext> AuraInputMappingContext;

	UPROPERTY(EditAnywhere, Category="Aura|Input")
	TObjectPtr<UInputAction> IA_Move;

	/* Function */
public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& InputActionValue);
};
