// Copyright fangh.space

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Untils/TickRate.h"
#include "AuraPlayerController.generated.h"

class UDamageTextWidgetComponent;
class USplineComponent;
class UAuraAbilitySystemComponent;
class UAuraInputComponent;
struct FInputActionValue;
struct FTimerHandle;
struct FGameplayTag;
class UDataAsset_AuraInputConfig;
class UInputAction;
class IEnemyInterface;
class UInputMappingContext;

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
	TObjectPtr<UInputAction> IA_SHIFT;
	
	bool bShiftKeyDown {false};

	UPROPERTY(EditAnywhere, Category="Aura|Input")
	TObjectPtr<UDataAsset_AuraInputConfig> InputConfig;

	UPROPERTY(BlueprintReadOnly, Category="Aura|Input", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAuraInputComponent> AuraInputComponent;

	UPROPERTY(BlueprintReadOnly, Category="Aura|GAS", meta=(AllowPrivateAccess=true))
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	// CursorTrace Tick Timer
	UPROPERTY(BlueprintReadWrite, Category="Aura|Tick|CursorTrace", meta=(AllowPrivateAccess=true))
	FTimerHandle TickTimerHandle_CursorTrace;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Tick|CursorTrace", meta=(AllowPrivateAccess=true))
	ETICK_RATE TickTimerRate_CursorTrace {ETICK_RATE::ER_TICK_10};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Tick|CursorTrace", meta=(AllowPrivateAccess=true))
	bool IsTickStart_CursorTrace {true};
	bool IsNativeTick_CursorTrace {false};

	// Mouse Trace Target Actor
	FHitResult CursorHitResult;
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

	UPROPERTY(VisibleDefaultsOnly, Category="Aura|TopDownMove")
	TObjectPtr<USplineComponent> SplineComponent;

	// AutoRun Tick Timer
	UPROPERTY(BlueprintReadWrite, Category="Aura|Tick|AutoRun", meta=(AllowPrivateAccess=true))
	FTimerHandle TickTimerHandle_AutoRun;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Tick|AutoRun", meta=(AllowPrivateAccess=true))
	ETICK_RATE TickTimerRate_AutoRun {ETICK_RATE::ER_TICK_Ultra};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aura|Tick|AutoRun", meta=(AllowPrivateAccess=true))
	bool IsTickStart_AutoRun {true};
	bool IsNativeTick_AutoRun {false};

	// DamageText Widget Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Aura|WidgetComponent", meta=(AllowPrivateAccess=true))
	TSubclassOf<UDamageTextWidgetComponent> DamageTextComponentClass;

	/* Function */
public:
	AAuraPlayerController();

	UFUNCTION(BlueprintPure, Category="Aura")
	UAuraAbilitySystemComponent* GetASComponent();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void SetupInputComponent() override;

	void Move(const FInputActionValue& InputActionValue);
	FORCEINLINE void ShiftPressed() {bShiftKeyDown = true;}
	FORCEINLINE void ShiftReleased() {bShiftKeyDown = false;}
	
	void CursorTrace();
	void AutoRun();

public:
	UFUNCTION(Client, Reliable, Category="Aura")
	void Client_ShowDamageNumber(const float DamageAmount, ACharacter* TargetCharacter, const bool IsBlockedHit, const bool IsCriticalHit);

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
