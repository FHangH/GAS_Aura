// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.generated.h"

class UDataAsset_AbilityInfo;
class UAttributeSet;
class UAbilitySystemComponent;
class AAuraPlayerController;
class AAuraPlayerState;
class UAuraAbilitySystemComponent;
class UAuraAttributeSet;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatusChangedSignature, int32, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, AbilityInfo);

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
	// Gameplay
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<APlayerState> PlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAbilitySystemComponent> ASComponent;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAttributeSet> AS;

	// Aura Gameplay
	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<AAuraPlayerController> AuraPlayerController;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<AAuraPlayerState> AuraPlayerState;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAuraAbilitySystemComponent> AuraASComponent;

	UPROPERTY(BlueprintReadOnly, Category="Aura")
	TObjectPtr<UAuraAttributeSet> AuraAS;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|WidgetController|DataAsset")
	TObjectPtr<UDataAsset_AbilityInfo> DataAsset_AbilityInfo;

	// Delegate
	UPROPERTY(BlueprintAssignable, Category="Aura|WidgetController|Messages")
	FAbilityInfoSignature OnAbilityInfoDelegate;

	/* Function */
public:
	// Get
	AAuraPlayerController* GetAuraPC();
	AAuraPlayerState* GetAuraPS();
	UAuraAbilitySystemComponent* GetAuraASC();
	UAuraAttributeSet* GetAuraAS();

	virtual void BindCallBackToDependencies();
	
	UFUNCTION(BlueprintCallable, Category="Aura")
	void SetWidgetControllerParams(const FWidgetControllerParams& Params);

	UFUNCTION(BlueprintCallable, Category="Aura")
	virtual void BroadcastInitValues();

	void BroadcastAbilityInfo();
};
