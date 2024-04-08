// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_USTRUCT_BODY()

	FWidgetControllerParams() {}
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) :
		PlayerController(PC), PlayerState(PS), ASComponent(ASC), AS(AS) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura")
	TObjectPtr<APlayerController> PlayerController {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura")
	TObjectPtr<APlayerState> PlayerState {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent {nullptr};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura")
	TObjectPtr<UAttributeSet> AS {nullptr};
};

UCLASS()
class GAS_AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

	/* Property */
protected:
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAttributeSet> AS;

	/* Function */
public:
	UFUNCTION(BlueprintCallable, Category="Aura")
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable, Category="Aura")
	virtual void BroadcastInitValues();

	virtual void BindCallBackToDependencies();
};
