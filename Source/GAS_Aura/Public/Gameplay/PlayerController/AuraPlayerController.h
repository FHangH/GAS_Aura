// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Untils/TickRate.h"
#include "AuraPlayerController.generated.h"

class USplineComponent;
class UAuraAbilitySystemComponent;
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Aura|GAS", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	// CursorTrace Tick Timer
	UPROPERTY(VisibleAnywhere, Category="Aura|Tick|CursorTrace")
	FTimerHandle TickTimerHandle_CursorTrace;
	
	UPROPERTY(EditAnywhere, Category="Aura|Tick|CursorTrace")
	ETICK_RATE TickTimerRate_CursorTrace {ETICK_RATE::ER_TICK_30};
	
	UPROPERTY(EditAnywhere, Category="Aura|Tick|CursorTrace")
	bool IsTickStart_CursorTrace {true};

	// Mouse Trace Target Actor
	IEnemyInterface* LastActor {nullptr};
	IEnemyInterface* ThisActor {nullptr};

	// Top Down Move For Mouse Click
	FVector CachedDestination {FVector::Zero()};
	float FollowTime {0.f};
	float ShortPressThreshold {0.5f};
	bool bAutoRunning {false};
	bool bTargeting {false};
	
	UPROPERTY(EditAnywhere, Category="Aura|TopDownMove|AutoRun")
	float AutoRunAcceptanceRadius {50.f};

	UPROPERTY(VisibleAnywhere, Category="Aura|TopDownMove")
	TObjectPtr<USplineComponent> SplineComponent;

	// AutoRun Tick Timer
	UPROPERTY(VisibleAnywhere, Category="Aura|Tick|AutoRun")
	FTimerHandle TickTimerHandle_AutoRun;
	
	UPROPERTY(EditAnywhere, Category="Aura|Tick|AutoRun")
	ETICK_RATE TickTimerRate_AutoRun {ETICK_RATE::ER_TICK_90};
	
	UPROPERTY(EditAnywhere, Category="Aura|Tick|AutoRun")
	bool IsTickStart_AutoRun {true};

	/* Function */
public:
	AAuraPlayerController();

	UFUNCTION(BlueprintPure, Category="Aura")
	UAuraAbilitySystemComponent* GetASComponent();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();
	void AutoRun();

private:
	// Tick Timer
	void TickHandle();
	// CursorTrace
	void InitTickTimerHandle_CursorTrace();
	void StartTickTimerHandle_CursorTrace() const;
	void StopTickTimerHandle_CursorTrace() const;
	void ClearTickTimerHandle_CursorTrace();
	// AutoRun
	void InitTickTimerHandle_AutoRun();
	void StartTickTimerHandle_AutoRun() const;
	void StopTickTimerHandle_AutoRun() const;
	void ClearTickTimerHandle_AutoRun();

	// Bind All Actions Use InputTag With DataAsset_AuraInputConfig
	void AbilityInputPressed(FGameplayTag InputTag);
	void AbilityInputReleased(FGameplayTag InputTag);
	void AbilityInputHeld(FGameplayTag InputTag);
};
