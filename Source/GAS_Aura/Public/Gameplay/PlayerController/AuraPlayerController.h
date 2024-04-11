// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Untils/TickRate.h"
#include "AuraPlayerController.generated.h"

class UAuraInputComponent;
struct FInputActionValue;
struct FTimerHandle;
struct FGameplayTag;
class UDataAsset_AuraInputConfig;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

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

	UPROPERTY(EditAnywhere, Category="Aura|Input")
	TObjectPtr<UDataAsset_AuraInputConfig> InputConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAuraInputComponent> AuraInputComponent;

	FTimerHandle PlayerControllerTickTimerHandle;

	UPROPERTY(EditAnywhere, Category="Aura|Tick")
	float TickTimerRate {TICK_60};

	UPROPERTY(EditAnywhere, Category="Aura|Tick")
	bool IsTickStart {true};

	IEnemyInterface* LastActor {nullptr};
	IEnemyInterface* ThisActor {nullptr};

	/* Function */
public:
	AAuraPlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

private:
	// Tick Timer Manager
	void TickHandle();
	void InitTickTimerHandle();
	void StartTickTimerHandle() const;
	void StopTickTimerHandle() const;
	void ClearTickTimerHandle();

	// Bind All Actions Use InputTag With DataAsset_AuraInputConfig
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
};
