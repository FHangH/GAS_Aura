// Copyright fangh.space

#include "Gameplay/PlayerController/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Untils/AuraLog.h"
#include "GameplayTagContainer.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!AuraInputMappingContext || !IA_Move)
	{
		UE_LOG(Aura, Warning, TEXT("%hc: AuraInputMappingContext or IA_Move is nullptr"), *__FUNCTION__);
		return;
	}
	
	if (const auto InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		InputSubsystem->AddMappingContext(AuraInputMappingContext, 0);
		
		bShowMouseCursor = true;
		DefaultMouseCursor = EMouseCursor::Default;
		
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
	}

	if (IsTickStart)
	{
		InitTickTimerHandle();
	}
}

void AAuraPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	ClearTickTimerHandle();
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	if (AuraInputComponent)
	{
		AuraInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
		AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputPressed, &ThisClass::AbilityInputReleased, &ThisClass::AbilityInputHeld);
	}
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const auto InputAxisVector = InputActionValue.Get<FVector2d>();
	const auto ForwardDirection =
		FRotationMatrix(FRotator{0.f, GetControlRotation().Yaw, 0.f}).GetUnitAxis(EAxis::X);
	const auto RightDirection =
		FRotationMatrix(FRotator{0.f, GetControlRotation().Yaw, 0.f}).GetUnitAxis(EAxis::Y);

	if (const auto Controlled = GetCharacter())
	{
		Controlled->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		Controlled->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHitResult);
	if (!CursorHitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHitResult.GetActor());

	/*
	 * Line Trace from Cursor. There are many ways to do this
	 * A. LastActor is null && ThisActor is null
	 *		- Do nothing
	 * B. LastActor is null && ThisActor is valid
	 *		- Highlight ThisActor
	 * C. LastActor is valid && ThisActor is null
	 *		- Unhighlight LastActor
	 * D. LastActor is valid && ThisActor is valid && ThisActor != LastActor
	 *		- Unhighlight LastActor And Highlight ThisActor
	 * E. LastActor is valid && ThisActor is valid && ThisActor == LastActor
	 *		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor!= nullptr)
		{
			// Case B
			ThisActor->HighLightActor();
		}
		else
		{
			// Case A - Do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor== nullptr)
		{
			// Case C
			LastActor->UnHighLightActor();
		}
		else // Both Actor Valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighLightActor();
				ThisActor->HighLightActor();
			}
			else
			{
				// Case E - Do nothing
			}
		}
	}
}

// Tick Timer Manager
void AAuraPlayerController::TickHandle()
{
	//UE_LOG(Aura, Warning, TEXT("%hc TickHandle - Rate: %f"), *__FUNCTION__, TickTimerRate);

	CursorTrace();
}

void AAuraPlayerController::InitTickTimerHandle()
{
	GetWorld()->GetTimerManager().SetTimer(
		PlayerControllerTickTimerHandle, this, &AAuraPlayerController::TickHandle, TickTimerRate, true);
}

void AAuraPlayerController::StartTickTimerHandle() const
{
	GetWorld()->GetTimerManager().UnPauseTimer(PlayerControllerTickTimerHandle);
}

void AAuraPlayerController::StopTickTimerHandle() const
{
	GetWorld()->GetTimerManager().PauseTimer(PlayerControllerTickTimerHandle);
}

void AAuraPlayerController::ClearTickTimerHandle()
{
	if (PlayerControllerTickTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(PlayerControllerTickTimerHandle);
	}
}

// Bind All Actions Use InputTag With DataAsset_AuraInputConfig
void AAuraPlayerController::AbilityInputPressed(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputReleased(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(2, 3.f, FColor::Blue, *InputTag.ToString());
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(3, 3.f, FColor::Green, *InputTag.ToString());
}
